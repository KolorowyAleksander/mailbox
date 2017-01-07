#include <postman/Connection.h>
#include <postman/Message.h>
#include <iostream>

int main(int argc, char* argv[]) {
  Connection connection("127.0.0.1", 1410);
  connection.send();
  std::cout << "Something's happening" << std::endl;
  return 0;
}
