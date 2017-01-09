#include <ConnectionReciever.h>
#include <SimpleLogger.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <string>

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

  std::string buffer;
  buffer.resize(100);
  if (read(_socket, &buffer[0], 100) < 0) {
    logger::log.error("Error while reading from buffer!", errno);
    return;
  }

  logger::log.info("Recieved: " + buffer);
}
