#include <ConnectionReciever.h>
#include <SimpleLogger.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  int fd;
  int port = 1410;

  if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    logger::log.error("Creating socket failed", errno);
    return 1;
  }

  int optval = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0) {
    std::cerr << "Setting socket options failed!" << std::endl;
    return 1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval) < 0) {
    std::cerr << "Setting socket options failed!" << std::endl;
    return 1;
  }
  sockaddr_in socketAddress = {AF_INET, htons(port), in_addr{INADDR_ANY}};

  if (bind(fd, (sockaddr*)&socketAddress, sizeof socketAddress) < 0) {
    std::cerr << "There's a problem with bind, capitain! " << std::endl;
    return 1;
  }

  if (listen(fd, -1) < 0) {
    std::cerr << "We cannot hear you!" << std::endl;
    return 1;
  }

  int newSocketFd;
  sockaddr_in inAddr;
  socklen_t inAddrSize = sizeof inAddr;
  if ((newSocketFd = accept(fd, (sockaddr*)&inAddr, &inAddrSize)) < 0) {
    std::cerr << "Accept failed! " << std::strerror(errno) << std::endl;
    return 1;
  } else {
    std::string buffer;
    buffer.resize(5);
    if (read(newSocketFd, &buffer[0], 5) < 0) {
      std::cerr << "Cannot read " << std::strerror(errno) << std::endl;
      return 1;
    }
    std::cout << buffer << std::endl;
  }

  std::cout << "Nawiązano połączenie!" << std::endl;
  logger::log.debug("Trying to log something");
  logger::log.error("Logging an error here");

  return 0;
}
