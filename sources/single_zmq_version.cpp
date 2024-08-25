#include <zmq.h>

#include <cstdlib>

#include "Logger.h"

int main(void) {
  int major;
  int minor;
  int patch;

  zmq_version(&major, &minor, &patch);
  LOG_DEB("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
  return EXIT_SUCCESS;
}
