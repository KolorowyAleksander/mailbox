#include <Queue.h>
#include <SimpleLogger.h>


typedef std::chrono::high_resolution_clock stopwatch;

Queue::Queue(std::string bindingKey, bool persistence, uint64_t durability)
    : _persistence{persistence}, _durability{durability} {}

void Queue::publish(std::vector<uint8_t> message) {
  _mutex.lock();
  _queue.push({std::move(message), stopwatch::now()});
  _mutex.unlock();
}

std::vector<uint8_t> Queue::collect() {
  _mutex.lock();
  std::vector<uint8_t> r;
  if (!_queue.empty() && (_queue.front().second + _durability <= stopwatch::now())) {
    r = _queue.front().first;
    _queue.pop();
  }
  _mutex.unlock();
  return r;
}
