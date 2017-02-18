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

Connection::Connection(std::string host, int port) : _port{port}, _host{host} {
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
  // big TODO: also everything in one piece!
  // TODO: validate routing key

  /*
   * Now this is required, cause if you don't include it, congestion control(?)
   * will kill your socket and you end up with a bad memory corrupton
   * This is a minor hack. Should be removed when there is a consumer/producer.
   */
  usleep(1000); // that is a thousand microseconds

  uint8_t tag = static_cast<uint8_t>(MessageTag::message);
  uint64_t messageSize = data.size();
  if (routingKey.size() > keySize) {
    throw PostmanConnectionException("Routing key must be less than " +
                                     std::to_string(keySize) +
                                     " characters.");
  }

  routingKey.resize(keySize);
  try {
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
  } catch (PostmanConnectionException e) {
    close(_socket);
    throw PostmanConnectionException("Cannot publish message");
  }
}

std::vector<uint8_t> Connection::collect() {
  //TODO: pulling messages
}

void Connection::ack() {
  //TODO: decide if this is even needed
}

void Connection::queueBind(std::string name) {
  // big TODO: send everything in one piece!
  // TODO: check broker if queue exists
  uint8_t tag = static_cast<uint8_t>(MessageTag::queueBind);
  if (name.size() > queueNameSize) {
    throw PostmanConnectionException("Name must be less than " +
                                     std::to_string(queueNameSize) +
                                     " characters.");
  }

  name.resize(queueNameSize);
  try {
    if (write(_socket, &tag, 1) < 0) {
      throw PostmanConnectionException("Cannot send binding tag.");
    }

    if (write(_socket, &name[0], queueNameSize) < 0) {
      throw PostmanConnectionException("Cannot send bind name.");
    }
  } catch (PostmanConnectionException e) {
    close(_socket);
    throw PostmanConnectionException("Cannot publish message");
  }
}

void Connection::queueDeclare(std::string name, std::string bindingKey,
                              bool persistence, bool durability) {
  // big TODO: send everything in one piece!
  // TODO: validate the binding key and name to be valuable
  // TODO: check for answear from broker if queue was created
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
                                     std::to_string(keySize) +
                                     " characters.");
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
  } catch (PostmanConnectionException e) {
    close(_socket);
    throw PostmanConnectionException("Cannot publish message");
  }
}
