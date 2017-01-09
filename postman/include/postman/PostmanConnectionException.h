#ifndef POSTMAN_CONNECTION_EXCEPTION_H
#define POSTMAN_CONNECTION_EXCEPTION_H

#include <postman/PostmanException.h>

class PostmanConnectionException : public PostmanException {
 public:
  PostmanConnectionException(std::string reason);
  PostmanConnectionException(std::string reason, int errnum);
};

#endif  // POSTMAN_CONNECTION_EXCEPTION_H
