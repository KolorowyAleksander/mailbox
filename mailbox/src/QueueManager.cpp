#include <QueueManager.h>
#include <SimpleLogger.h>
#include <iostream>

QueueManager manager;

QueueManager::QueueManager() {
  // TODO: read persistent queues to read here
  // TODO: a mapping to binding
}

void QueueManager::publish(std::string routing_key,
                           std::vector<uint8_t> message) {
  logger::log.info("A message was published with key " + routing_key + ", " +
                   std::string(message.begin(), message.end()));
  // TODO: find all the queues and publish your message
}

std::shared_ptr<Queue> QueueManager::queueBinding(std::string name) {
  // TODO: handle errors
  return _queues.find(name)->second;
}

void QueueManager::queueInit(std::string name, std::string bindingKey,
                             bool persistence, bool durability) {
  // TODO: check if queue exists first
  // TODO: handle errors when creating queues
  logger::log.info("New queue declared: " + name + ",  " + bindingKey + ", " +
                   std::to_string(persistence) + std::to_string(durability));
  _mutex.lock();
  _queues.insert({name, std::shared_ptr<Queue>(
                            new Queue(bindingKey, persistence, durability))});
  _mutex.unlock();
}
