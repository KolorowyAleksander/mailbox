#include <QueueManager.h>
#include <SimpleLogger.h>
#include <utilities.h>

#include <algorithm>
#include <iostream>
#include <sstream>

QueueManager manager;

QueueManager::QueueManager() {
  // TODO: read persistent queues to read here
}

void QueueManager::publish(std::string routingKey,
                           std::vector<uint8_t> message) {
  for (auto& i : _queueBindings) {
    if (utilities::matching(routingKey, i.second)) {
      _queues[i.first]->publish(std::move(message));
    }
  }
}

std::shared_ptr<Queue> QueueManager::queueBinding(std::string name) {
  auto result = _queues.find(name);
  return result == _queues.end() ? nullptr : result->second;
}

int QueueManager::queueInit(std::string name, std::string bindingKey,
                            bool persistence, bool durability) {
  auto result =
      std::find_if(_queueBindings.begin(), _queueBindings.end(),
                   [name](const std::pair<std::string, std::string>& element) {
                     return element.first == name;
                   });

  if (result != _queueBindings.end()) {
    logger::log.error("The queue with name " + name + " already exists");
    return -1;
  }

  logger::log.info("New queue declared: " + name + ", " + bindingKey);

  _mutex.lock();
  _queues[name] =
      std::shared_ptr<Queue>(new Queue(bindingKey, persistence, durability));
  _queueBindings.push_back({name, bindingKey});
  _mutex.unlock();
  return 0;
}
