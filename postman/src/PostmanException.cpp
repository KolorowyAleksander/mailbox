#include <postman/PostmanException.h>
#include <exception>
PostmanException::PostmanException(std::string reason)
    : std::runtime_error(reason){};
