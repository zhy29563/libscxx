#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "Logger.h"

sem_t sem;
void *start_routine_01([[maybe_unused]] void *ptr) {
  sem_wait(&sem);
  for (size_t i = 0; i < 5; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem);

  return nullptr;
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  sem_wait(&sem);
  for (size_t i = 10; i < 15; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem);

  return nullptr;
}

void *start_routine_03([[maybe_unused]] void *ptr) {
  sem_wait(&sem);
  for (size_t i = 20; i < 25; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem);

  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  sem_init(&sem, 0, 1);

  pthread_t thread_id_01;
  pthread_t thread_id_02;
  pthread_t thread_id_03;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  sem_destroy(&sem);
  return EXIT_SUCCESS;
}
