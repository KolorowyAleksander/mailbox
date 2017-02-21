#include <postman/Connection.h>
#include <postman/PostmanException.h>
#include <unistd.h>
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
  std::string broker = std::string(argv[1]);

  Connection connection(broker, 1410);
  connection.queueDeclare("name", "lele.*.morele", true, true);

  std::thread t2([broker]() {
    try {
      Connection connection(broker, 1410);
      connection.queueBind("name");
      for (;;) {
        std::vector<uint8_t> res = connection.collect();
        std::string b(res.begin(), res.end());
        std::cout << b << std::endl;
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread t1([broker]() {
    try {
      Connection connection(broker, 1410);
      std::string s = {"This is mine message!"};
      std::vector<uint8_t> data(s.begin(), s.end());

      for (;;) {
        connection.publish(data, "lele.trele.morele");
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  t1.join();
  t2.join();

  return 0;
}
