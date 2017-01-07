#include <client/Message.h>

Message::Message(std::string message) { this->message = message; }

std::string Message::getMessage() { return message; }
