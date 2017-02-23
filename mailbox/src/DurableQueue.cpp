#include <DurableQueue.h>
#include <SimpleLogger.h>

DurableQueue::DurableQueue(std::string bindingKey, bool persistence)
    : Queue(bindingKey, persistence) {}

void DurableQueue::publish(std::vector<uint8_t> message) {
  _mutex.lock();
  _queue.push(std::move(message));
  _mutex.unlock();
}

std::vector<uint8_t> DurableQueue::collect() {
  std::vector<uint8_t> r;
  _mutex.lock();
  if (!_queue.empty()) {
    r = _queue.front();
    _queue.pop();
  }
  _mutex.unlock();
  return r;
}
