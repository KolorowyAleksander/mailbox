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
const int keySize = 255;

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
  // big TODO: assert that proper number of bytes is read
  // big TODO: handle whole deliveries in one go
  logger::log.info("New connection from: " + _host + " on " +
                   std::to_string(_port));

  while (true) {
    uint8_t tag;
    int recieved = read(_socket, &tag, 1);

    if (recieved < 0) {
      logger::log.error("Error while reading tag socket!", errno);
      return;
    } else if (recieved > 0) {
      switch (MessageTag(tag)) {
        case MessageTag::message:
          handleMessageDelivery();
          break;
        case MessageTag::collect:
        break;
        case MessageTag::queueBind:
          handleQueueBinding();
        break;
        case MessageTag::queueDeclare:
          handleQueueDeclaration();
          break;
        default: {
          logger::log.error("Unhandled tag was read from socket!", errno);
          return;
        }
      }
    }
  }

  close(_socket);
}

void ConnectionReciever::handleMessageDelivery() {
  uint64_t size;
  std::string routingKey;
  routingKey.resize(keySize);

  if (read(_socket, &size, 8) < 0) {
    logger::log.error("Error while reading size from socket!", errno);
    return;
  }

  if (read(_socket, &routingKey[0], keySize) < 0) {
    logger::log.error("Error while reading key from socket!", errno);
    return;
  }

  routingKey.shrink_to_fit();
  std::vector<uint8_t> buffer;
  buffer.resize(size);
  if (read(_socket, &buffer[0], size) < 0) {
    logger::log.error("Error while reading from socket!", errno);
    return;
  }

  manager.publish(routingKey, std::move(buffer));
}

void ConnectionReciever::handleMessageCollection() {}

void ConnectionReciever::handleQueueBinding() {
  std::string name;
  name.resize(queueNameSize);

  if (read(_socket, &name[0], queueNameSize) < 0) {
    logger::log.error("Error while reading queue name from socket!", errno);
    return;
  }

  name.shrink_to_fit();
  _queue = manager.queueBinding(name);
}

void ConnectionReciever::handleQueueDeclaration() {
  std::string name;
  name.resize(queueNameSize);
  std::string bindingKey;
  bindingKey.resize(keySize);
  bool persistence;
  bool durability;

  if (read(_socket, &persistence, 1) < 0) {
    logger::log.error("Error while reading persistence flag from socket!",
                      errno);
    return;
  }

  if (read(_socket, &durability, 1) < 0) {
    logger::log.error("Error while reading durability flag from socket!",
                      errno);
    return;
  }

  if (read(_socket, &name[0], queueNameSize) < 0) {
    logger::log.error("Error while reading queue name from socket!", errno);
    return;
  }

  if (read(_socket, &bindingKey[0], keySize) < 0) {
    logger::log.error("Error while reading size from socket!", errno);
    return;
  }

  name.shrink_to_fit();
  bindingKey.shrink_to_fit();
  manager.queueInit(name, bindingKey, persistence, durability);
}
