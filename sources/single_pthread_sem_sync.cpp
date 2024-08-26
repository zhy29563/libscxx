#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "Logger.h"

sem_t sem_01, sem_02, sem_03;

void *start_routine_01([[maybe_unused]] void *ptr) {
  sem_wait(&sem_01);
  for (size_t i = 0; i < 5; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_02);

  return nullptr;
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  sem_wait(&sem_02);
  for (size_t i = 10; i < 15; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_03);

  return nullptr;
}

void *start_routine_03([[maybe_unused]] void *ptr) {
  sem_wait(&sem_03);
  for (size_t i = 20; i < 25; i++) {
    LOG_DEB("读线程(%lu)获取全局变量当前值(%lu)", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_01);

  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  sem_init(&sem_01, 0, 1);
  sem_init(&sem_02, 0, 0);
  sem_init(&sem_03, 0, 0);

  pthread_t thread_id_01;
  pthread_t thread_id_02;
  pthread_t thread_id_03;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  sem_destroy(&sem_01);
  sem_destroy(&sem_02);
  sem_destroy(&sem_03);
  return EXIT_SUCCESS;
}
