#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>

class Message {
 private:
  std::vector<unsigned char> message;

 public:
  Message(std::vector<unsigned char> message);
  Message(void * data, size_t size);
  std::string getMessage();
};

#endif  // MESSAGE_H
