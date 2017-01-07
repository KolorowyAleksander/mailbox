#include <client/Connection.h>
#include <client/Message.h>
#include <iostream>

int main(int argc, char* argv[]) {
  Message message("Here is your message");
  std::cout << "I'm sending this: " << message.getMessage() << std::endl;
  Connection connection("127.0.0.1", 1410);
  connection.sendMessage(message);
  std::cout << "Something's happening" << std::endl;
  return 0;
}
