#include <client/Connection.h>

Connection::Connection(std::string host, int port)
    : port{port}, hostName{host} {}

void Connection::connect() {}

void Connection::send(Message message) {}
