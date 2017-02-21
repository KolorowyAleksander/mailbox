#include <postman/Connection.h>
#include <postman/PostmanException.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    // TODO: maybe add something nicer?
    Connection connection(argv[1], 1410);
    std::string s = {"This is mine message!"};
    std::vector<uint8_t> data(s.begin(), s.end());

    connection.queueDeclare("name", "lele.trele.morele", true, true);
    for (int i = 0; i < 10; i++) {
      connection.publish(data, "lele.trele.morele");
    }

    connection.queueBind("name");
    for (int i = 0; i < 10; i++) {
      std::vector<uint8_t> res = connection.collect();
      std::string b(res.begin(), res.end());
      std::cout << b << std::endl;
    }

  } catch (PostmanException e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
