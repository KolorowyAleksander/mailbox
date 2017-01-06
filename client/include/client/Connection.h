#ifndef CONNECTION_H
#define CONNECTION_H

#include <client/message.h>
#include <string>

class Connection {
private:
  int socketFd;
  int port;
  std::string hostName;

 public:
  Connection(std::string host, int port);
  void connect();
  void send(Message message);
};

#endif  // CONNECTION_H
