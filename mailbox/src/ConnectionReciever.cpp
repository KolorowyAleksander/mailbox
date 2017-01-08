#include <ConnectionReciever.h>
#include <SimpleLogger.h>

void ConnectionReciever::operator()() {
  logger::log.info("I'm inside ConnectionReciever!");
}
