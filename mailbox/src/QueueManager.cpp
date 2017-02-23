#include <QueueManager.h>
#include <SimpleLogger.h>
#include <utilities.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

QueueManager::QueueManager(std::string queuesFileName) : _file(queuesFileName) {
  std::ifstream readStream;
  readStream.open(_file);
  if (!readStream.is_open()) {
    logger::log.error("Queues file doesn't exist yet!");
  } else {
    std::string line;
    while (getline(readStream, line)) {
      std::vector<std::string> queueParameters = utilities::split(line, '|');
      std::string name = queueParameters[0];
      std::string bindingKey = queueParameters[1];
      bool persistence = (queueParameters[2] == "1");
      bool durability = (queueParameters[3] == "1");
      this->insertQueue(name, bindingKey, persistence, durability);
    }
  }
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
                            bool persistence, uint64_t durability) {
  auto result =
      std::find_if(_queueBindings.begin(), _queueBindings.end(),
                   [name](const std::pair<std::string, std::string>& element) {
                     return element.first == name;
                   });

  if (result != _queueBindings.end()) {
    logger::log.error("The queue with name " + name + " already exists");
    return -1;
  }

  if (result == _queueBindings.end() && persistence) {
    std::ofstream writeStream;
    writeStream.open(_file, std::ios::app);
    writeStream << name << "|" << bindingKey << "|" << persistence << "|"
                << durability << std::endl;
    writeStream.close();
  }

  logger::log.info("New queue declared: " + name + ", " + bindingKey + ", " + std::to_string(persistence) + ", " + std::to_string(durability));
  this->insertQueue(name, bindingKey, persistence, durability);

  return 0;
}

void QueueManager::insertQueue(std::string name, std::string bindingKey,
                               bool persistence, uint64_t durability) {
  _mutex.lock();
  _queues[name] =
      std::shared_ptr<Queue>(new Queue(bindingKey, persistence, durability));
  _queueBindings.push_back({name, bindingKey});
  _mutex.unlock();
}
