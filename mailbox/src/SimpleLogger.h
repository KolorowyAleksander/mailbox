#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

namespace logger {

class Log {
 public:
  Log(std::string filename);
  ~Log();
  void error(std::string message);
  void error(std::string message, int errno);
  void debug(std::string message);
  void info(std::string message);

 private:
  std::mutex lock;
  std::string logFileName;
  std::ofstream logFile;
};

extern Log log;
}

#endif  // SIMPLE_LOGGER_H
