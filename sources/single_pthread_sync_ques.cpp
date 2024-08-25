#include <pthread.h>  // pthread_create pthread_t
#include <stdio.h>    // printf fprintf
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

int g_num = 0;

void *start_routine_01(void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    g_num++;
  }

  return (void *)"8888";
}

void *start_routine_02(void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    g_num++;
  }

  return (void *)"9999";
}

int main(int argc, char const *argv[]) {
  pthread_t thread_id_01;
  pthread_t thread_id_02;

  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_01, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  printf("计算结果为： %d\n", g_num);

  printf("主线程即将退出...\n");
  exit(EXIT_SUCCESS);
}