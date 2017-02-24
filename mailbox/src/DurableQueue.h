#ifndef DURABLE_QUEUE_H
#define DURABLE_QUEUE_H

#include <Queue.h>

#include <condition_variable>


class DurableQueue : public Queue {
 public:
  DurableQueue(std::string bindingKey, bool persistence);
  void publish(std::vector<uint8_t> message) override;
  std::vector<uint8_t> collect() override;

 private:
  std::queue<std::vector<uint8_t> > _queue;
  std::condition_variable _cv;
};

#endif  // DURABLE_QUEUE_H
