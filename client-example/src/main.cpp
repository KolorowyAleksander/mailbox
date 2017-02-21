#include <postman/Connection.h>
#include <postman/PostmanException.h>
#include <unistd.h>
#include <iostream>
#include "longMessage.h"

int main(int argc, char* argv[]) {
  try {
    // TODO: maybe add something nicer?
    Connection connection(argv[1], 1410);
    std::string s = {"This is mine message!"};

    std::vector<uint8_t> data(s.begin(), s.end());

    connection.queueDeclare("name", "lele.trele.morele", true, true);
    for (int i = 0; i < 10; i++) {
      connection.publish(data, "lele.trele.morele");
      usleep(1000);
    }


    connection.queueBind("name");
    for (int i = 0; i < 10; i++) {
      std::vector<uint8_t> res = connection.collect();
      std::string b(res.begin(), res.end());
      std::cout << b << std::endl;
    }

    // std::vector<uint8_t> longMessage(longString.begin(), longString.end());
    // connection.publish(longMessage, "lele.trele.morele");

    // std::vector<uint8_t> se = connection.collect();
    // std::string sr(se.begin(), se.end());

    // std::cout << sr <<std::endl;
    // std::cout << (sr == longString) << std::endl;
  } catch (PostmanException e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
