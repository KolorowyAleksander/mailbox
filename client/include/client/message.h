#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
 private:
  std::string message;

 public:
  Message(std::string);
  std::string getMessage();
};

#endif  // MESSAGE_H
