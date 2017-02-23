#include <postman/Connection.h>
#include <postman/PostmanException.h>

#include <unistd.h>

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "long_message.h"
const int port = 1410;

// taken from http://stackoverflow.com/a/24586587 and modified a little
std::string random_string(unsigned int length) {
  static auto& letters =
      "0123456789"
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  thread_local static std::mt19937 rg{std::random_device{}()};
  thread_local static std::uniform_int_distribution<std::string::size_type>
      pick(0, sizeof(letters) - 2);

  std::string s;
  s.reserve(length);

  while (length--) {
    s += letters[pick(rg)];
  }

  return s;
}

int main(int argc, char* argv[]) {
  std::string broker = std::string(argv[1]);

  Connection connection(broker, port);
  connection.queueDeclare("timed_queue_test", "time", false, 100);
  connection.queueDeclare("owoce", "*.trele.morele", true, 0);
  connection.queueDeclare("warzywa", "pomidory.#", false, 0);
  connection.queueDeclare("007", "i.am.#", false, 0);
  connection.queueDeclare("poczta_polska", "no.message.will.ever.get.here.ever",
                          false, 0);

  try {
    Connection secondConnection(broker, port);
    for (int i = 0; i < 5; i++) {
      std::string message = std::to_string(i) + "xxxxxxxxxxx";
      std::vector<uint8_t> v(message.begin(), message.end());
      secondConnection.publish(v, "time");
    }
  } catch (PostmanException e) {
    std::cout << e.what() << std::endl;
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::thread timedReciever([broker]() {
    try {
      Connection connection(broker, port);
      connection.queueBind("timed_queue_test");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::cout << "recieved message from timed queue!" << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread fruitDealer([broker]() {
    std::string workerName{"Kim Won Ki"};
    try {
      Connection connection(broker, port);
      for (int i = 0; i < 77777; i++) {
        std::string fruit = (i % 3 == 0 ? "banana" : "apple");
        std::vector<uint8_t> v(fruit.begin(), fruit.end());
        connection.publish(v, fruit + ".trele.morele");
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread m([broker]() {
    std::string workerName{"M"};
    try {
      Connection connection(broker, port);
      connection.queueBind("007");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread theNewM([broker]() {
    std::string workerName{"The New M"};
    try {
      Connection connection(broker, port);
      connection.queueBind("007");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread james([broker]() {
    std::string workerName{"James"};
    try {
      Connection connection(broker, port);
      for (auto i = 0; i < 10000; i++) {
        std::string message = workerName + "--" + random_string(107);
        std::vector<uint8_t> v(message.begin(), message.end());
        connection.publish(v, "i.am.bond");
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  fruitDealer.join();

  std::thread bond([broker]() {
    std::string workerName{"Bond"};
    try {
      Connection connection(broker, port);
      for (auto i = 0; i < 10000; i++) {
        std::string message = workerName + "--" + random_string(100);
        std::vector<uint8_t> v(message.begin(), message.end());
        connection.publish(v, "i.am.james.bond");
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread annoyingPoster([broker]() {
    std::string workerName{"Gniewko"};
    try {
      Connection connection(broker, port);
      while (true) {
        std::string message = workerName + "--" + long_message;
        std::vector<uint8_t> v(long_message.begin(), long_message.end());
        connection.publish(v, "no.message.will.ever.get.here");
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread miraclePostalWorker([broker]() {
    std::string workerName{"Pat"};
    try {
      Connection connection(broker, port);
      connection.queueBind("poczta_polska");
      while (true) {
        std::string message = workerName + "--" + random_string(100);
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread reposter([broker]() {
    std::string workerName{"Jester the Protester"};
    try {
      Connection connection(broker, port);
      connection.queueBind("owoce");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        connection.publish(v, "pomidory.morelowe");
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread monkey([broker]() {
    std::string workerName{"Curious George"};
    try {
      Connection connection(broker, port);
      connection.queueBind("owoce");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  std::thread fly([broker]() {
    std::string workerName{"Muszka Owocufka"};
    try {
      Connection connection(broker, port);
      connection.queueBind("owoce");
      while (true) {
        std::vector<uint8_t> v = connection.collect();
        if (v.size() != 0) {
          std::string message(v.begin(), v.end());
          std::cout << workerName << " recieved: " << message << std::endl;
        }
      }
    } catch (PostmanException e) {
      std::cout << e.what() << std::endl;
    }
  });

  m.join();
  theNewM.join();
  miraclePostalWorker.join();
  reposter.join();
  james.join();
  bond.join();
  annoyingPoster.join();
  monkey.join();
  fly.join();
  return 0;
}
