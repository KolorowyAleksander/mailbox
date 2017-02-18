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

  /*
   * Publish a message (byte vector) to the exchange. It will then be
   * propagated to queues.
   * data - a vector of bytes to be sent
   * routingKey - a key which will be used to match the message to queues
   * message can be propagated to many queues or none
   */
  void publish(std::vector<uint8_t> data, std::string routingKey);
  /*
   * Declare a queue you want to create on the broker
   * name - a string with max 255 characters identifying the queue
   * bindingKey - a string with max 255 characters which will be used to match
   * messages to the queue
   * persistence - the ability for the queue to survive broker restarts/crashes
   * durability - durable queues will keep messages safe from restarts/crashes
   * but at a cost of speed and space
   */
  void queueDeclare(std::string name, std::string bindingKey, bool persistence,
                    bool durability);
  /*
   * Collect a message from bound queue.
   */
  void collect();

  /*
   * Acknowledge the message has been recieved.
   * If there is no acknowledgement, the broker will requeue the message,
   * treating it as if it was never delivered.
   */
  void ack();
};

#endif  // CONNECTION_H
