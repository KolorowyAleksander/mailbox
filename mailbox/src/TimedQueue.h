#ifndef TIMED_QUEUE_H
#define TIMED_QUEUE_H

#include <Queue.h>
#include <chrono>

class TimedQueue : public Queue {
 public:
  TimedQueue(std::string bindingKey, bool persistence, uint64_t durability);
  void publish(std::vector<uint8_t> message) override;
  std::vector<uint8_t> collect() override;

 private:
  std::queue<std::pair<std::vector<uint8_t>,
                       std::chrono::high_resolution_clock::time_point> > _queue;
  std::chrono::milliseconds _durability;
};

#endif  // TIMED_QUEUE_H
