#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "Logger.h"

pthread_barrier_t barrier;

void *start_routine_01([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)开始运行...", pthread_self());
  LOG_DEB("子线程(%lu)栅栏等待状态前", pthread_self());
  pthread_barrier_wait(&barrier);
  LOG_DEB("子线程(%lu)栅栏等待状态后", pthread_self());
  LOG_DEB("子线程(%lu)即将退出...", pthread_self());
  return static_cast<void *>(const_cast<char *>("9999"));
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)开始运行...", pthread_self());
  LOG_DEB("子线程(%lu)栅栏等待状态前", pthread_self());
  pthread_barrier_wait(&barrier);
  LOG_DEB("子线程(%lu)栅栏等待状态后", pthread_self());
  LOG_DEB("子线程(%lu)即将退出...", pthread_self());
  return static_cast<void *>(const_cast<char *>("9999"));
}

void *start_routine_03([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)开始运行...", pthread_self());
  LOG_DEB("子线程(%lu)栅栏等待状态前", pthread_self());
  pthread_barrier_wait(&barrier);
  LOG_DEB("子线程(%lu)栅栏等待状态后", pthread_self());
  LOG_DEB("子线程(%lu)即将退出...", pthread_self());
  return static_cast<void *>(const_cast<char *>("9999"));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  LOG_DEB("主线程(%lu)开始运行...", pthread_self());

  pthread_barrierattr_t attr;
  pthread_barrierattr_init(&attr);
  pthread_barrier_init(&barrier, &attr, 3);
  pthread_barrierattr_destroy(&attr);

  pthread_t thread_id_01;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);

  pthread_t thread_id_02;
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);

  pthread_t thread_id_03;
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  pthread_barrier_destroy(&barrier);
  LOG_DEB("主线程(%lu)即将退出...", pthread_self());
  return EXIT_SUCCESS;
}
