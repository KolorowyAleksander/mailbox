#ifndef CONNECTION_RECIEVER_H
#define CONNECTION_RECIEVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

/*
 * a connection reciever class - we use it to run a new thread with state inside
 */

class ConnectionReciever {
 public:
  ConnectionReciever(int socket, sockaddr_in addr);
  void operator()();

 private:
  int _socket;  // connected socket file descriptor
  std::string _host;
  int _port;
};

#endif  // CONNECTION_RECIEVER_H
