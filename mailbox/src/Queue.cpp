#include <Queue.h>
#include <SimpleLogger.h>

Queue::Queue(std::string bindingKey, bool persistence, bool durability) {
  // TODO: here goes the whole queue mechanism
  // TODO: messages durability
}

void Queue::publish(std::vector<uint8_t> message) {
  logger::log.info("Recieved message: " +
                   std::string(message.begin(), message.end()));
  // TODO: accepting messages
  // TODO: maybe queue should be a friend funtion for manager?
}

std::vector<uint8_t> Queue::collect() {
  // TODO: get the next message from queue
  // This is just temp message being sent here.
  std::string s{"This is a return message!"};
  return std::vector<uint8_t>(s.begin(), s.end());
}
