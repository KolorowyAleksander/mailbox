#include <QueueManager.h>
#include <SimpleLogger.h>
#include <iostream>

QueueManager manager;

QueueManager::QueueManager() {
  // TODO:
  // read persistent queues to read here
  // also what about durability
}

std::shared_ptr<Queue> QueueManager::queueInit(std::string name,
                                               std::string bindingKey,
                                               bool persistence,
                                               bool durability) {
  // TODO: check if queue exists first
  logger::log.info("New queue declared: " + name + ",  " + bindingKey + ", " +
                   std::to_string(persistence) + std::to_string(durability));
  _mutex.lock();
  _queues.insert({name, std::shared_ptr<Queue>(
                            new Queue(bindingKey, persistence, durability))});
  _mutex.unlock();
}

void QueueManager::publish(std::string routing_key,
                           std::vector<uint8_t> message) {
  // TODO: find all the queues and publish your message
}
