#ifndef CONNECTION_H
#define CONNECTION_H

#include <postman/Message.h>
#include <string>

class Connection {
 private:
  int _socket;
  std::string _host;
  int _port;

 public:
  Connection(std::string host, int port);
  ~Connection();
  void publish(std::vector<uint8_t> data, std::string bindingKey);
  void recieve();
  void ack();
};

#endif  // CONNECTION_H
