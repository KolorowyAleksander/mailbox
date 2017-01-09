#include <postman/Connection.h>
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

void Connection::publish(std::vector<uint8_t> data, std::string bindingKey) {
  uint8_t tag = 06;
  uint64_t messageSize = data.size();
  try {
    if (write(_socket, &tag, 1) < 0) {
      throw PostmanConnectionException("Cannot publish message tag.");
    }

    if (write(_socket, &messageSize, 8) < 0) {
      throw PostmanConnectionException("Cannot publish message size.");
    }

    if (write(_socket, &data[0], data.size()) < 0) {
      throw PostmanConnectionException("Cannot publish message.");
    }
  } catch (PostmanConnectionException e) {
    close(_socket);
    throw PostmanConnectionException("Cannot publish message");
  }
}

void Connection::recieve() {}

void Connection::ack() {}
