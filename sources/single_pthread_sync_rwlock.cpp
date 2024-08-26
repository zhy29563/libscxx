#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "Logger.h"

int              g_num = 0;
pthread_rwlock_t rwlock;

void *start_routine_01([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_rdlock(&rwlock);
    for (size_t i = 0; i < 2; i++) {
      LOG_DEB("读线程(%lu)获取全局变量当前值(%d)", pthread_self(), g_num);
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }

  return nullptr;
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_rdlock(&rwlock);
    for (size_t i = 0; i < 2; i++) {
      LOG_DEB("读线程(%lu)获取全局变量当前值(%d)", pthread_self(), g_num);
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }

  return nullptr;
}

void *start_routine_03([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_wrlock(&rwlock);
    LOG_DEB("写线程(%lu)设置全局变量当前值(%d)", pthread_self(), ++g_num);
    pthread_rwlock_unlock(&rwlock);
    sleep(2);
  }

  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  pthread_rwlockattr_t attr;
  pthread_rwlockattr_init(&attr);
  pthread_rwlock_init(&rwlock, &attr);
  pthread_rwlockattr_destroy(&attr);

  pthread_t thread_id_01;
  pthread_t thread_id_02;
  pthread_t thread_id_03;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  pthread_rwlock_destroy(&rwlock);
  return EXIT_SUCCESS;
}
