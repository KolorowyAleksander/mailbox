#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <Queue.h>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class QueueManager {
 public:
  QueueManager(std::string queuesFileName);

  void publish(std::string routingKey, std::vector<uint8_t> message);
  std::shared_ptr<Queue> queueBinding(std::string name);
  int queueInit(std::string name, std::string bindingKey, bool persistence,
                uint64_t durability);

 private:
  std::unordered_map<std::string, std::shared_ptr<Queue> > _queues;
  std::vector<std::pair<std::string, std::string> > _queueBindings;
  std::mutex _mutex;
  const std::string _file;

  void insertQueue(std::string name, std::string bindingKey, bool persistence,
                   uint64_t durability);
};

#endif  // QUEUE_MANAGER_H
