#include <postman/Connection.h>
#include <postman/PostmanException.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
  try {
    Connection connection("127.0.0.1", 1410);
    std::string s = {"This is mine message!"};
    std::vector<uint8_t> data(s.begin(), s.end());

    connection.queueDeclare("name", "lele", true, true);
    while(true) {
      connection.publish(data, "lele");
      usleep(100);
    }

  } catch (PostmanException e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
