#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_barrier_t barrier;

void *start_routine_01(void *ptr) {
  printf("子线程(%lu)开始运行...\n", pthread_self());
  printf("子线程(%lu)栅栏等待状态前\n", pthread_self());
  pthread_barrier_wait(&barrier);
  printf("子线程(%lu)栅栏等待状态后\n", pthread_self());
  printf("子线程(%lu)即将退出...\n", pthread_self());
  return (void *)"9999";
}

void *start_routine_02(void *ptr) {
  printf("子线程(%lu)开始运行...\n", pthread_self());
  printf("子线程(%lu)栅栏等待状态前\n", pthread_self());
  pthread_barrier_wait(&barrier);
  printf("子线程(%lu)栅栏等待状态后\n", pthread_self());
  printf("子线程(%lu)即将退出...\n", pthread_self());
  return (void *)"9999";
}

void *start_routine_03(void *ptr) {
  printf("子线程(%lu)开始运行...\n", pthread_self());
  printf("子线程(%lu)栅栏等待状态前\n", pthread_self());
  pthread_barrier_wait(&barrier);
  printf("子线程(%lu)栅栏等待状态后\n", pthread_self());
  printf("子线程(%lu)即将退出...\n", pthread_self());
  return (void *)"9999";
}

int main(int argc, char const *argv[]) {
  printf("主线程(%lu)开始运行...\n", pthread_self());

  pthread_barrierattr_t attr;
  pthread_barrierattr_init(&attr);
  pthread_barrier_init(&barrier, &attr, 3);
  pthread_barrier_destroy(&attr);

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
  printf("主线程(%lu)即将退出...\n", pthread_self());
  exit(EXIT_SUCCESS);
}