#include <arpa/inet.h>
#include <netinet/in.h>
#include <postman/Connection.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

Connection::Connection(std::string host, int port)
    : port{port}, hostName{host} {
  if ((socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    std::cerr << "Error creating socket" << std::endl;
    // TODO: throw an exception here
  }

  sockaddr_in socketAddress = {AF_INET, htons(port),
                               in_addr{inet_addr(hostName.c_str())}};
  socklen_t socketAddressLenght = sizeof socketAddress;
  if (::connect(socketFd, (sockaddr *)&socketAddress, socketAddressLenght) <
      0) {
    std::cerr << "Error connectioning" << std::endl;
    // TODO: throw an exception here
  }
}

void Connection::connect() {}

void Connection::send() {
  std::vector<unsigned char> buffer = {'0', '0', 'F', 'F'};
  if (::send(socketFd, &buffer[0], 4, 0) < 0) {
    std::cerr << "Error sending" << std::endl;
    // TODO: throw an exception here
  }
}

void Connection::recieve() {}

void Connection::ack() {}
