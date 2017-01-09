#include <ConnectionReciever.h>
#include <SimpleLogger.h>
#include <postman/MessageTag.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <vector>

ConnectionReciever::ConnectionReciever(int socket, sockaddr_in addr)
    : _socket{socket},
      _host{std::string(inet_ntoa(addr.sin_addr))},
      _port{addr.sin_port} {}

ConnectionReciever::ConnectionReciever(ConnectionReciever &&other)
    : _socket{other._socket},
      _host{std::move(other._host)},
      _port{other._port} {}

ConnectionReciever &ConnectionReciever::operator=(ConnectionReciever &&other) {
  _socket = other._socket;
  _host = std::move(other._host);
  _port = other._port;
  return *this;
}

void ConnectionReciever::operator()() {
  logger::log.info("New connection from: " + _host + " on " +
                   std::to_string(_port));

  uint8_t tag;
  if (read(_socket, &tag, 1) < 0) {
    logger::log.error("Error while reading tag socket!", errno);
    return;
  }

  uint64_t size;
  if (read(_socket, &size, 8) < 0) {
    logger::log.error("Error while reading size from socket!", errno);
    return;
  }

  std::vector<uint8_t> buffer;
  buffer.reserve(size);
  if (read(_socket, &buffer[0], 100) < 0) {
    logger::log.error("Error while reading from socket!", errno);
    return;
  }

  std::string message(buffer.begin(), buffer.begin() + size);
  if (tag == static_cast<uint8_t>(MessageTag::message)) {
    //we know that we handle message here!
    logger::log.info("Recieved message! It says: " + message);
  }
}
