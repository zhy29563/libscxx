#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_01, sem_02, sem_03;

void *start_routine_01(void *ptr) {
  sem_wait(&sem_01);
  for (size_t i = 0; i < 5; i++) {
    printf("读线程(%lu)获取全局变量当前值(%lu)\n", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_02);

  return (void *)NULL;
}

void *start_routine_02(void *ptr) {
  sem_wait(&sem_02);
  for (size_t i = 10; i < 15; i++) {
    printf("读线程(%lu)获取全局变量当前值(%lu)\n", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_03);

  return (void *)NULL;
}

void *start_routine_03(void *ptr) {
  sem_wait(&sem_03);
  for (size_t i = 20; i < 25; i++) {
    printf("读线程(%lu)获取全局变量当前值(%lu)\n", pthread_self(), i);
    sleep(1);
  }
  sem_post(&sem_01);

  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  sem_init(&sem_01, 0, 1);
  sem_init(&sem_02, 0, 0);
  sem_init(&sem_03, 0, 0);

  pthread_t thread_id_01, thread_id_02, thread_id_03;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  sem_destroy(&sem_01);
  sem_destroy(&sem_02);
  sem_destroy(&sem_03);
  exit(EXIT_SUCCESS);
}