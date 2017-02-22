#include <ConnectionReciever.h>
#include <QueueManager.h>
#include <SimpleLogger.h>

#include <postman/MessageTag.h>

#include <arpa/inet.h>
#include <unistd.h>

#include <cerrno>
#include <vector>

const int queueNameSize = 255;
const int keySize = 255;

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

void trim(std::string &a) {
  auto position = a.find_first_of((char)0);
  a.erase(a.begin() + position, a.end());
}

ConnectionReciever::ConnectionReciever(std::shared_ptr<QueueManager> manager,
                                       int socket, sockaddr_in addr)
    : _manager{manager},
      _socket{socket},
      _host{std::string(inet_ntoa(addr.sin_addr))},
      _port{addr.sin_port} {}

ConnectionReciever::ConnectionReciever(ConnectionReciever &&other)
    : _manager{other._manager},
      _socket{other._socket},
      _host{other._host},
      _port{other._port} {}

ConnectionReciever &ConnectionReciever::operator=(ConnectionReciever &&other) {
  _manager = other._manager;
  _socket = other._socket;
  _host = other._host;
  _port = other._port;
  return *this;
}

void ConnectionReciever::operator()() {
  logger::log.info("New connection from: " + _host + " on " +
                   std::to_string(_port));

  while (true) {
    uint8_t tag;
    int recieved = read(_socket, &tag, 1);
    if (recieved == 0) {
      logger::log.info("Connection with " + _host + " ended.");
      return;
    }

    if (recieved < 0) {
      logger::log.error("Error while reading tag from socket!", errno);
      return;
    }

    switch (MessageTag(tag)) {
      case MessageTag::message:
        handleMessageDelivery();
        break;
      case MessageTag::collect:
        handleMessageCollection();
        break;
      case MessageTag::queueBind:
        handleQueueBinding();
        break;
      case MessageTag::queueDeclare:
        handleQueueDeclaration();
        break;
    }
  }

  close(_socket);
}

void ConnectionReciever::handleMessageDelivery() {
  std::vector<uint8_t> sizeBytes;
  if (readFromSocket(_socket, sizeBytes, 8) < 0) {
    logger::log.error("Error while reading size from socket!", errno);
    return;
  }

  std::vector<uint8_t> routingKeyBytes;
  if (readFromSocket(_socket, routingKeyBytes, keySize) < 0) {
    logger::log.error("Error while reading key from socket!", errno);
    return;
  }

  uint64_t size = *reinterpret_cast<uint64_t*>(sizeBytes.data());
  std::vector<uint8_t> buffer;
  if (readFromSocket(_socket, buffer, size) < 0) {
    logger::log.error("Error while reading from socket!", errno);
    return;
  }

  std::string routingKey(routingKeyBytes.begin(), routingKeyBytes.end());
  trim(routingKey);

  _manager->publish(routingKey, std::move(buffer));
}

void ConnectionReciever::handleMessageCollection() {
  if (!_queue) {
    logger::log.error("Recieved read while queue not bound!", errno);
    return;
  }

  std::vector<uint8_t> message = _queue->collect();
  uint64_t size = message.size();

  if (write(_socket, &size, 8) < 0) {
    logger::log.error("Failed to send size!", errno);
    return;
  }

  if (write(_socket, &message[0], size) < 0) {
    logger::log.error("Failed to send message!", errno);
    return;
  }
}

void ConnectionReciever::handleQueueBinding() {
  std::vector<uint8_t> queueNameBytes;
  if (readFromSocket(_socket, queueNameBytes, queueNameSize) < 0) {
    logger::log.error("Error while reading queue name from socket!", errno);
    return;
  }

  std::string queueName(queueNameBytes.begin(), queueNameBytes.end());
  trim(queueName);

  uint8_t tag;
  if ((_queue = _manager->queueBinding(queueName))) {
    tag = static_cast<uint8_t>(MessageTag::ack);
  } else {
    tag = static_cast<uint8_t>(MessageTag::rej);
  }

  if (write(_socket, &tag, 1) < 0) {
    logger::log.error("Failed to send queue ack or rej!", errno);
    return;
  }
}

void ConnectionReciever::handleQueueDeclaration() {
  bool persistence;
  if (read(_socket, &persistence, 1) < 0) {
    logger::log.error("Error while reading persistence from socket!", errno);
    return;
  }

  bool durability;
  if (read(_socket, &durability, 1) < 0) {
    logger::log.error("Error while reading durability from socket!", errno);
    return;
  }

  std::vector<uint8_t> queueNameBytes;
  if (readFromSocket(_socket, queueNameBytes, queueNameSize) < 0) {
    logger::log.error("Error while reading queue name from socket!", errno);
    return;
  }

  std::vector<uint8_t> bindingKeyBytes;
  if (readFromSocket(_socket, bindingKeyBytes, keySize) < 0) {
    logger::log.error("Error while reading size from socket!", errno);
    return;
  }

  std::string queueName(queueNameBytes.begin(), queueNameBytes.end());
  std::string bindingKey(bindingKeyBytes.begin(), bindingKeyBytes.end());
  trim(queueName);
  trim(bindingKey);

  uint8_t tag = static_cast<uint8_t>(MessageTag::ack);
  _manager->queueInit(queueName, bindingKey, persistence, durability);

  if (write(_socket, &tag, 1) < 0) {
    logger::log.error("Failed to acknowledge queue declaration!", errno);
    return;
  }
}
