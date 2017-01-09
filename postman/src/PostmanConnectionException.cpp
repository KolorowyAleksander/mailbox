#include <postman/PostmanConnectionException.h>
#include <cstring>

PostmanConnectionException::PostmanConnectionException(std::string reason)
    : PostmanException(reason) {}

PostmanConnectionException::PostmanConnectionException(std::string reason,
                                                       int errnum)
    : PostmanException(reason + strerror(errnum)) {}
