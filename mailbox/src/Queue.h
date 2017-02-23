#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <chrono>


class Queue {
 public:
  Queue(std::string bindingKey, bool persistence, uint64_t durability);
  void publish(std::vector<uint8_t> message);
  std::vector<uint8_t> collect();

 private:
  std::queue<std::pair<std::vector<uint8_t>, std::chrono::high_resolution_clock::time_point> > _queue;
  std::mutex _mutex;
  bool _persistence;
  std::chrono::milliseconds _durability;
};

#endif  // QUEUE_H
