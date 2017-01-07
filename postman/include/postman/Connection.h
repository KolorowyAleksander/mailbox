#ifndef CONNECTION_H
#define CONNECTION_H

#include <postman/Message.h>
#include <string>

class Connection {
 private:
  int socketFd;
  int port;
  std::string hostName;

 public:
  Connection(std::string host, int port);
  void connect(); //::global namespace operator
  void send(); //global namespace operator
  void recieve();
  void ack();
};

#endif  // CONNECTION_H
