#include <pthread.h>  // pthread_create pthread_t
#include <stdio.h>    // printf fprintf
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

void *start_routine(void *ptr) {
  printf("子线程的标识：%lu\n", pthread_self());
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  printf("主线程的标识：%lu\n", pthread_self());

  int iret = 0;
  pthread_t thread_id;

  iret = pthread_create(&thread_id, NULL, start_routine, NULL);
  if (iret) {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
    exit(EXIT_FAILURE);
  }

  printf("主线程等待5秒后结束...\n");
  sleep(5);

  printf("主线程即将退出...\n");
  exit(EXIT_SUCCESS);
}