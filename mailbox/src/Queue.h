#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>
#include <string>
#include <vector>

class Queue {
 public:
  Queue(std::string bindingKey, bool persistence);
  virtual void publish(std::vector<uint8_t> message) = 0;
  virtual std::vector<uint8_t> collect() = 0;
  std::string bindingKey;

 protected:
  std::mutex _mutex;
  bool _persistence;
};

#endif  // ABSTRACT_QUEUE_H
