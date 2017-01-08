#ifndef CONNECTION_RECIEVER_H
#define CONNECTION_RECIEVER_H

/*
 * a connection reciever class - we use it to run a new thread with state inside
 */

class ConnectionReciever {
public:
  void operator()();
private:
  int _socket; // connected socket file descriptor
};

#endif //CONNECTION_RECIEVER_H
