#include <message.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <iostream>

int main(int argc, char* argv[]) {
  int fd;
  int port = 1410;

  if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    std::cerr << "Error creating socket" << std::endl;
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
  sockaddr_in inSocketAddress;
  socklen_t inSocketAddressSize = sizeof inSocketAddress;
  if ((newSocketFd = accept(fd, (sockaddr*)&inSocketAddress,
                            &inSocketAddressSize)) < 0) {
    std::cerr << "Accepting a socket failed! " << std::strerror(errno)
              << std::endl;
    return 1;
  }

  
  std::cout << "Nawiązano połączenie!" << std::endl;

  Message exampleMessage("I'm alive!");
  std::cout << exampleMessage.getMessage() << "\n";
  return 0;
}
