#include <pthread.h>  // pthread_create pthread_t
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

#include "Logger.h"

void *start_routine([[maybe_unused]] void *ptr) {
  int num = 0;
  while (true) {
    LOG_DEB("子线程正在执行，%d...", ++num);
    sleep(1);
  }

  return static_cast<void *>(const_cast<char *>("9999"));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  int       error_code = 0;
  pthread_t thread_id;

  error_code = pthread_create(&thread_id, NULL, start_routine, NULL);
  if (0 != error_code) {
    LOG_DEB("pthread_create() return code: %d", error_code);
    return EXIT_FAILURE;
  }

  LOG_DEB("主线程等待5秒后结束...");
  sleep(5);

  LOG_DEB("主线程即将退出...");
  return EXIT_SUCCESS;
}
