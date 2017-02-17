#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <vector>

class Connection {
 private:
  int _socket;
  std::string _host;
  int _port;

 public:
  Connection(std::string host, int port);
  ~Connection();
  void publish(std::vector<uint8_t> data, std::string routingKey);
  void queueDeclare(std::string name, std::string bindingKey, bool persistence,
                    bool durability);
  void recieve();
  void ack();
};

#endif  // CONNECTION_H
