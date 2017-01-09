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

  int socket, port = 1410;

  if ((socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    return errorHappened("Socket was not created!");
  }

  int opt = 1;
  if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0) {
    return errorHappened("Setting socket reuseaddr options failed!");
  }

  sockaddr_in socketAddress = {AF_INET, htons(port), in_addr{INADDR_ANY}};
  if (bind(socket, (sockaddr*)&socketAddress, sizeof socketAddress) < 0) {
    return errorHappened("There's a problem with bind, capitain!");
  }

  if (listen(socket, -1) < 0) {
    return errorHappened("We cannot hear you!");
  }

  int newSocket;
  sockaddr_in inAddr;
  socklen_t inAddrSize = sizeof inAddr;
  while ((newSocket = accept(socket, (sockaddr*)&inAddr, &inAddrSize)) != -1) {
    std::thread((ConnectionReciever(newSocket, inAddr))).detach();
  }

  return 0;
}
