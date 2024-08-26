#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "Logger.h"

int exit_flag = 0;

void *start_routine([[maybe_unused]] void *ptr) {
  do {
    LOG_DEB("++++++++++++++++++++++++++");
    usleep(500000);
  } while (0 == exit_flag);

  return static_cast<void *>(const_cast<char *>("9999"));
}

void exit_func() {
  LOG_DEB("------------------------------------");
  exit_flag = 1;
  usleep(2000000);
  LOG_DEB("====================================");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  int a = atexit(exit_func);
  LOG_DEB("atexit return a = %d", a);
  LOG_DEB("主线程(%lu)即将退出...", pthread_self());
  return EXIT_SUCCESS;
}
