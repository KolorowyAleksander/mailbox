#ifndef CONNECTION_RECIEVER_H
#define CONNECTION_RECIEVER_H

#include <Queue.h>
#include <QueueManager.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <memory>
#include <string>

class ConnectionReciever {
 public:
  ConnectionReciever(std::shared_ptr<QueueManager> manager, int socket,
                     sockaddr_in addr);
  ConnectionReciever(ConnectionReciever &&other);
  ConnectionReciever &operator=(ConnectionReciever &&other);
  void operator()();

 private:
  int _socket;
  std::string _host;
  int _port;
  std::shared_ptr<Queue> _queue;
  std::shared_ptr<QueueManager> _manager;

  void handleMessageDelivery();
  void handleMessageCollection();
  void handleQueueDeclaration();
  void handleQueueBinding();
};

#endif  // CONNECTION_RECIEVER_H
