#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "Logger.h"

pthread_spinlock_t spinlock;

void *start_routine_01([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)开始运行...", pthread_self());
  for (size_t i = 0; i < 3; i++) {
    pthread_spin_lock(&spinlock);
    LOG_DEB("子线程(%lu)进入自旋锁", pthread_self());
    sleep(1);
    LOG_DEB("子线程(%lu)离开自旋锁", pthread_self());
    pthread_spin_unlock(&spinlock);
  }
  return static_cast<void *>(const_cast<char *>("9999"));
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)开始运行...", pthread_self());
  for (size_t i = 0; i < 3; i++) {
    pthread_spin_lock(&spinlock);
    LOG_DEB("子线程(%lu)进入自旋锁", pthread_self());
    sleep(1);
    LOG_DEB("子线程(%lu)离开自旋锁", pthread_self());
    pthread_spin_unlock(&spinlock);
  }
  return static_cast<void *>(const_cast<char *>("9999"));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  LOG_DEB("主线程(%lu)开始运行...", pthread_self());

  pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

  pthread_t thread_id_01;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);

  pthread_t thread_id_02;
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  pthread_spin_destroy(&spinlock);
  LOG_DEB("主线程(%lu)即将退出...", pthread_self());
  return EXIT_SUCCESS;
}
