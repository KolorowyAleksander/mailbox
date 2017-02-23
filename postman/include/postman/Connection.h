#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include <vector>

class Connection {
 private:
  int _socket;
  std::string _host;
  int _port;
  bool _isBound;

 public:
  /*
   * Creates a connection to a broker on given host/port.
   */
  Connection(std::string host, int port);
  ~Connection();

  /*
   * Publish a message (byte vector) to the exchange. It will then be
   * propagated to queues.
   * data - a vector of bytes to be sent
   * routingKey - a key which will be used to match the message to queues
   * A message can be propagated to many queues or none.
   */
  void publish(std::vector<uint8_t> data, std::string routingKey);

  /*
   * Collect a message from bound queue.
   * If no queue is bound, this will throw an exception.
   * Returns the recieved message.
   * Throws an exception if something went awry.
   */
  std::vector<uint8_t> collect();

  /*
   * Binds the connection to a queue on the broker.
   * This is only required to recieve messages, messages are sent to exchange
   * instead.
   */
  void queueBind(std::string name);

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
                    uint64_t durability);
};

#endif  // CONNECTION_H
