#ifndef CONNECTION_RECIEVER_H
#define CONNECTION_RECIEVER_H

#include <Queue.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <memory>
#include <string>

/*
 * a connection reciever class - we use it to run a new thread with state inside
 */

class ConnectionReciever {
 public:
  ConnectionReciever(int socket, sockaddr_in addr);
  ConnectionReciever(ConnectionReciever &&other);
  ConnectionReciever &operator=(ConnectionReciever &&other);
  void operator()();

 private:
  int _socket;  // connected socket file descriptor
  std::string _host;
  int _port;
  std::shared_ptr<Queue> _queue;
};

#endif  // CONNECTION_RECIEVER_H
