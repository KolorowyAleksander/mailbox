#include <Queue.h>
#include <SimpleLogger.h>

Queue::Queue(std::string bindingKey, bool persistence, bool durability) {
  // TODO: here goes the whole queueing logic
}

void Queue::publish(std::vector<uint8_t> message) {
  std::string s(message.begin(), message.begin() + message.size());
  logger::log.info("Recieved message! It says: " + s);
  // TODO: accepting messages
  // TODO: maybe queue should be a friend funtion for manager?
}

void Queue::pull() {
  // TODO: get the next message from queue
}
