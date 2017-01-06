#include <iostream>
#include <client/Connection.h>

int main(int argc, char* argv[]) {
  Connection connection("127.0.0.1", 1410);
  std::cout << "Something's happening" << std::endl;
  return 0;
}
