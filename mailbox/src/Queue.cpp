#include <Queue.h>
#include <SimpleLogger.h>


Queue::Queue(std::string bindingKey, bool persistence, bool durability) {
  // TODO: messages durability
}

void Queue::publish(std::vector<uint8_t> message) {
  logger::log.info("Recieved message: " +
                   std::string(message.begin(), message.end()));
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
