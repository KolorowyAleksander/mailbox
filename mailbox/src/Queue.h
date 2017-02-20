#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>
#include <string>
#include <vector>

class Queue {
 public:
  Queue(std::string bindingKey, bool persistence, bool durability);
  void publish(std::vector<uint8_t> message);
  std::vector<uint8_t> collect();

 private:
  std::queue<std::vector<uint8_t> > _queue;
  std::mutex _mutex;
};

#endif  // QUEUE_H
