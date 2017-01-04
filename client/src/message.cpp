#include <message.h>

Message::Message(std::string message) {
  this->message = message;
}

std::string Message::getMessage() {
  return message;
}
