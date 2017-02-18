#include <ConnectionReciever.h>
#include <QueueManager.h>
#include <SimpleLogger.h>

#include <postman/MessageTag.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <vector>

#include <iostream>

const int queueNameSize = 255;
const int bindingKeySize = 255;

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
  // TODO: handle different cases depending on tag
  // big TODO: assert that proper number of bytes is read
  // big TODO: handle whole deliveries in one go
  logger::log.info("New connection from: " + _host + " on " +
                   std::to_string(_port));

  while (true) {
    uint8_t tag;
    int recieved = read(_socket, &tag, 1);
    if (recieved == 0) {
      continue;
    }
    if (recieved < 0) {
      logger::log.error("Error while reading tag socket!", errno);
      return;
    }

    switch (MessageTag(tag)) {
      case MessageTag::message: {
        uint64_t size;
        if (read(_socket, &size, 8) < 0) {
          logger::log.error("Error while reading size from socket!", errno);
          return;
        }
        std::vector<uint8_t> buffer;
        buffer.resize(size);
        if (read(_socket, &buffer[0], 100) < 0) {
          logger::log.error("Error while reading from socket!", errno);
          return;
        }

        if (!_queue) {
          logger::log.error("Recieved message, but queue is not declared!");
        } else {
          _queue->publish(std::move(buffer));
        }

        break;
      }
      case MessageTag::queueDeclare: {
        if (MessageTag(tag) == MessageTag::queueDeclare) {
          std::string name;
          name.resize(queueNameSize);
          std::string bindingKey;
          bindingKey.resize(bindingKeySize);
          bool persistence;
          bool durability;

          if (read(_socket, &persistence, 1) < 0) {
            logger::log.error(
                "Error while reading persistence flag from socket!", errno);
            return;
          }

          if (read(_socket, &durability, 1) < 0) {
            logger::log.error(
                "Error while reading durability flag from socket!", errno);
            return;
          }

          if (read(_socket, &name[0], queueNameSize) < 255) {
            logger::log.error("Error while reading queue name from socket!",
                              errno);
            return;
          }

          if (read(_socket, &bindingKey[0], bindingKeySize) < 255) {
            logger::log.error("Error while reading size from socket!", errno);
            return;
          }

          name.shrink_to_fit();
          bindingKey.shrink_to_fit();
          _queue = manager.queueInit(name, bindingKey, persistence, durability);
        }
        break;
      }
      default: {
        logger::log.error("Unhandled tag was read from socket!", errno);
        return;
      }
    }
  }
  close(_socket);
}
