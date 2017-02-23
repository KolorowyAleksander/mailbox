#include <Queue.h>


Queue::Queue(std::string bindingKey, bool persistence) :
  bindingKey{bindingKey}, _persistence{persistence} {}
