#include <postman/Connection.h>
#include <postman/MessageTag.h>
#include <postman/PostmanConnectionException.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

const int queueNameSize = 255;
const int keySize = 255;

Connection::Connection(std::string host, int port)
    : _port{port}, _host{host}, _isBound(false) {
  if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    throw PostmanConnectionException("Cannot create socket.");
  }

  sockaddr_in addr = {AF_INET, htons(_port), in_addr{inet_addr(_host.c_str())}};
  socklen_t addrSize = sizeof addr;
  if (::connect(_socket, (sockaddr *)&addr, addrSize) < 0) {
    throw PostmanConnectionException("Cannot connect to the socket");
  }
}

Connection::~Connection() { close(_socket); }

void Connection::publish(std::vector<uint8_t> data, std::string routingKey) {
  // TODO: validate routing key

  uint8_t tag = static_cast<uint8_t>(MessageTag::message);
  uint64_t messageSize = data.size();
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
  // TODO: handle ack
  if (!this->_isBound) {
    throw PostmanConnectionException("Queue is not bound!");
  }

  uint8_t tag = static_cast<uint8_t>(MessageTag::collect);
  if (write(_socket, &tag, 1) < 0) {
    throw PostmanConnectionException("Cannot collection message tag.");
  }

  uint64_t size;
  if (read(_socket, &size, 8) < 0) {
    throw PostmanConnectionException("Cannot collection message tag.");
  }

  std::vector<uint8_t> message;
  message.resize(size);
  if (read(_socket, &message[0], size) < 0) {
    throw PostmanConnectionException("Cannot collection message tag.");
  }

  return std::move(message);
}

void Connection::queueBind(std::string name) {
  // big TODO: send everything in one piece!
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

  uint8_t tag;
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
                              bool persistence, bool durability) {
  // TODO: validate the binding key and name to be valuable
  uint8_t tag = static_cast<uint8_t>(MessageTag::queueDeclare);
  uint8_t per = persistence;
  uint8_t dur = durability;

  if (name.size() > queueNameSize) {
    throw PostmanConnectionException("Name must be less than " +
                                     std::to_string(queueNameSize) +
                                     " characters.");
  }

  if (bindingKey.size() > keySize) {
    throw PostmanConnectionException("Key must be less than " +
                                     std::to_string(keySize) + " characters.");
  }

  name.resize(queueNameSize);
  bindingKey.resize(keySize);

  try {
    if (write(_socket, &tag, 1) < 0) {
      throw PostmanConnectionException("Cannot send declaration tag.");
    }

    if (write(_socket, &per, 1) < 0) {
      throw PostmanConnectionException("Cannot send declaration persistence.");
    }

    if (write(_socket, &dur, 1) < 0) {
      throw PostmanConnectionException("Cannot send declaration durability.");
    }

    if (write(_socket, &name[0], queueNameSize) < 0) {
      throw PostmanConnectionException("Cannot send declaration name.");
    }

    if (write(_socket, &bindingKey[0], keySize) < 0) {
      throw PostmanConnectionException("Cannot publish message.");
    }

    uint8_t tag;
    int result = read(_socket, &tag, 1);

    if (result < 0) {
      throw PostmanConnectionException("Error recieving declare respnonse.");
    }

    if (MessageTag(tag) != MessageTag::ack) {
      throw PostmanConnectionException("Error recieving declare.");
    }

  } catch (PostmanConnectionException e) {
    close(_socket);
    throw PostmanConnectionException(e.what());
  }
}
