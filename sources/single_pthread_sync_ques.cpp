#include <pthread.h>  // pthread_create pthread_t
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

#include "Logger.h"

int g_num = 0;

void *start_routine_01([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    g_num++;
  }

  return static_cast<void *>(const_cast<char *>("8888"));
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    g_num++;
  }

  return static_cast<void *>(const_cast<char *>("9999"));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  pthread_t thread_id_01;
  pthread_t thread_id_02;

  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  LOG_DEB("计算结果为： %d", g_num);
  LOG_DEB("主线程即将退出...");

  return EXIT_SUCCESS;
}
