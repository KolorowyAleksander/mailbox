#include <Queue.h>
#include <SimpleLogger.h>

Queue::Queue(std::string bindingKey, bool persistence, bool durability)
    : _persistence(persistence), _durability(durability) {}

void Queue::publish(std::vector<uint8_t> message) {
  _mutex.lock();
  _queue.push(std::move(message));
  _mutex.unlock();
}

std::vector<uint8_t> Queue::collect() {
  _mutex.lock();
  std::vector<uint8_t> r;
  if (!_queue.empty()) {
    r = _queue.front();
    _queue.pop();
  }
  _mutex.unlock();
  return r;
}
