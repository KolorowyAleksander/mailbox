#include <ConnectionReciever.h>
#include <SimpleLogger.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <thread>

int errorHappened(std::string message) {
  logger::log.error(message, errno);
  return 1;
}

int main(int argc, char* argv[]) {
  logger::log.info("Constructing mailbox.");

  int fd;
  int port = 1410;

  if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    return errorHappened("Socket was not created!");
  }

  int optval = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0) {
    return errorHappened("Setting socket reuseaddr options failed!");
  }

  if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval) < 0) {
    return errorHappened("Setting socket options failed!");
  }
  sockaddr_in socketAddress = {AF_INET, htons(port), in_addr{INADDR_ANY}};

  if (bind(fd, (sockaddr*)&socketAddress, sizeof socketAddress) < 0) {
    return errorHappened("There's a problem with bind, capitain!");
  }

  if (listen(fd, -1) < 0) {
    return errorHappened("We cannot hear you!");
  }

  sockaddr_in inAddr;
  socklen_t inAddrSize = sizeof inAddr;

  int newSocketFd;
  while ((newSocketFd = accept(fd, (sockaddr*)&inAddr, &inAddrSize)) != -1) {
    std::thread t((ConnectionReciever(newSocketFd, inAddr)));
    t.detach();
  }
  return 0;
}
