#include <SimpleLogger.h>
#include <TimedQueue.h>

typedef std::chrono::high_resolution_clock stopwatch;

TimedQueue::TimedQueue(std::string bindingKey, bool persistence,
                       uint64_t durability)
    : Queue(bindingKey, persistence), _durability{durability} {}

void TimedQueue::publish(std::vector<uint8_t> message) {
  _mutex.lock();
  _queue.push({std::move(message), stopwatch::now()});
  _mutex.unlock();
}

std::vector<uint8_t> TimedQueue::collect() {
  std::vector<uint8_t> r;
  // TODO check for time properly
  while (!_queue.empty() &&
         (_queue.front().second + _durability <= stopwatch::now())) {
    _mutex.lock();
    r = _queue.front().first;
    _queue.pop();
    _mutex.unlock();
    return r;
  }
}
