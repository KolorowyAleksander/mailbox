#include <iostream>
#include <message.h>

int main(int argc, char* argv[]) {
  Message exampleMessage("I'm alive!");
  std::cout << exampleMessage.getMessage() << "\n";
  return 0;
}
