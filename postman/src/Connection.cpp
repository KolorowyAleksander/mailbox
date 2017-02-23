#include <postman/Connection.h>
#include <postman/MessageTag.h>
#include <postman/PostmanConnectionException.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <regex>
#include <string>
#include <vector>

const int queueNameSize = 255;
const int keySize = 255;
const std::regex routingKeyPattern("\\w+(\\.\\w+)*");
const std::regex namePattern("(\\w|\\d)+");
const std::regex bindingKeyPattern("(\\w+|\\*|\\#)(\\.(\\w+|\\*|\\#))*");

int readFromSocket(int sck, std::vector<uint8_t> &v, unsigned int size) {
  v.resize(size);
  int n, toRead = size;
  while ((n = read(sck, &v[size - toRead], toRead)) > 0) {
    toRead -= n;
    if (n <= 0 && toRead != 0) {
      return -1;
    }
  }
}

Connection::Connection(std::string host, int port)
    : _port{port}, _host{host}, _isBound(false) {
  if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    throw PostmanConnectionException("Cannot create socket.");
  }

  int optval = 1;
  int optlen = sizeof(optval);
  if (setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
    close(_socket);
    throw PostmanConnectionException("Setting keepalive failed");
  }

  sockaddr_in addr = {AF_INET, htons(_port), in_addr{inet_addr(_host.c_str())}};
  socklen_t addrSize = sizeof addr;
  if (::connect(_socket, (sockaddr *)&addr, addrSize) < 0) {
    close(_socket);
    throw PostmanConnectionException("Cannot connect to the socket");
  }
}

Connection::~Connection() { close(_socket); }

void Connection::publish(std::vector<uint8_t> data, std::string routingKey) {
  uint8_t tag = static_cast<uint8_t>(MessageTag::message);
  uint64_t messageSize = data.size();

  if (!std::regex_match(routingKey, routingKeyPattern)) {
    throw PostmanConnectionException("Invalid routing key");
  }

  if (routingKey.size() > keySize) {
    throw PostmanConnectionException("Routing key must be less than " +
                                     std::to_string(keySize) + " characters.");
  }

  routingKey.resize(keySize);
  if (write(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Cannot publish message tag.");
  }

  if (write(_socket, &messageSize, 8) < 0) {
    throw PostmanConnectionException("Cannot publish message size.");
  }

  if (write(_socket, &routingKey[0], 255) < 0) {
    throw PostmanConnectionException("Cannot publish message routing key.");
  }

  if (write(_socket, &data[0], data.size()) < 0) {
    throw PostmanConnectionException("Cannot publish message.");
  }
}

std::vector<uint8_t> Connection::collect() {
  if (!this->_isBound) {
    throw PostmanConnectionException("Queue is not bound!");
  }

  uint8_t tag = static_cast<uint8_t>(MessageTag::collect);
  if (write(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Cannot collect message tag.");
  }

  std::vector<uint8_t> sizeBytes;
  if (readFromSocket(_socket, sizeBytes, 8) < 0) {
    throw PostmanConnectionException("Cannot collect message size.");
  }

  uint64_t size = *reinterpret_cast<uint64_t*> (sizeBytes.data());
  std::vector<uint8_t> message;
  if (readFromSocket(_socket, message, size) < 0) {
    throw PostmanConnectionException("Cannot collect message.");
  }

  return std::move(message);
}

void Connection::queueBind(std::string name) {
  uint8_t tag = static_cast<uint8_t>(MessageTag::queueBind);
  if (name.size() > queueNameSize) {
    throw PostmanConnectionException("Name must be less than " +
                                     std::to_string(queueNameSize) +
                                     " characters.");
  }

  name.resize(queueNameSize);
  if (write(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Cannot send binding tag.");
  }

  if (write(_socket, &name[0], queueNameSize) < 0) {
    throw PostmanConnectionException("Cannot send bind name.");
  }

  if (read(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Didn't recieve tag.");
  }

  if (MessageTag(tag) == MessageTag::ack) {
    this->_isBound = true;
  } else {
    throw PostmanConnectionException("The queue doesn't exist.");
  }
}

void Connection::queueDeclare(std::string name, std::string bindingKey,
                              bool persistence, uint64_t durability) {

  if (!std::regex_match(name, namePattern)) {
    throw PostmanConnectionException("Name contains strange characters!");
  }
  if (!std::regex_match(bindingKey, bindingKeyPattern)) {
    throw PostmanConnectionException("Binding key is wrong!");
  }

  if (name.size() > queueNameSize) {
    throw PostmanConnectionException("Name must be less than " +
                                     std::to_string(queueNameSize) +
                                     " characters.");
  }

  if (bindingKey.size() > keySize) {
    throw PostmanConnectionException("Key must be less than " +
                                     std::to_string(keySize) + " characters.");
  }

  uint8_t tag = static_cast<uint8_t>(MessageTag::queueDeclare);
  uint8_t per = persistence;
  uint64_t dur = durability;

  name.resize(queueNameSize);
  bindingKey.resize(keySize);

  if (write(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Cannot send declaration tag.");
  }

  if (write(_socket, &per, 1) < 0) {
    throw PostmanConnectionException("Cannot send declaration persistence.");
  }

  if (write(_socket, &dur, 8) < 0) {
    throw PostmanConnectionException("Cannot send declaration durability.");
  }

  if (write(_socket, &name[0], queueNameSize) < 0) {
    throw PostmanConnectionException("Cannot send declaration name.");
  }

  if (write(_socket, &bindingKey[0], keySize) < 0) {
    throw PostmanConnectionException("Cannot publish message.");
  }

  int result = read(_socket, &tag, 1);

  if (result < 0) {
    throw PostmanConnectionException("Error recieving declare respnonse.");
  }

  if (MessageTag(tag) != MessageTag::ack) {
    throw PostmanConnectionException("Error recieving declare.");
  }
}
