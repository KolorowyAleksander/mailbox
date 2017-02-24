#include <DurableQueue.h>
#include <SimpleLogger.h>

DurableQueue::DurableQueue(std::string bindingKey, bool persistence)
    : Queue(bindingKey, persistence) {}

void DurableQueue::publish(std::vector<uint8_t> message) {
  std::unique_lock<std::mutex> lock (_mutex);
  lock.unlock();
  _queue.push(std::move(message));
  _cv.notify_one();
  lock.lock();
}

std::vector<uint8_t> DurableQueue::collect() {
  std::vector<uint8_t> r;
  std::unique_lock<std::mutex> lock(_mutex);
  
  _cv.wait(lock,[this](){return !_queue.empty();});
  r = _queue.front();
  _queue.pop();
  return r;
}
