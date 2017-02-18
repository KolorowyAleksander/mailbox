#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <vector>

class Queue {
 public:
  Queue(std::string bindingKey, bool persistence, bool durability);
  void publish(std::vector<uint8_t> message);
  std::vector<uint8_t> collect();
  // TODO:
  // a data structure to hold messages
  // persistence
  // durability
  // disposing messages between workers
  // concurrency control (?)
};

#endif  // QUEUE_H
