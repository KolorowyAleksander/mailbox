#ifndef POSTMAN_EXCEPTION_H
#define POSTMAN_EXCEPTION_H

#include <exception>
#include <stdexcept>

class PostmanException : public std::runtime_error {
 protected:
  PostmanException(std::string reason);
};

#endif  // POSTMAN_EXCEPTION_H
