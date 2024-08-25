#include <pthread.h>  // pthread_create pthread_t
#include <stdio.h>    // printf fprintf
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

void *start_routine(void *ptr) {
  printf("子线程等待3秒后退出...\n");
  sleep(3);
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  int iret = 0;
  pthread_t thread_id;

  iret = pthread_create(&thread_id, NULL, start_routine, NULL);
  if (iret) {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
    exit(EXIT_FAILURE);
  }
  printf("主线程继续执行...\n");

  iret = pthread_detach(thread_id);
  if (iret) {
    fprintf(stderr, "Error - pthread_detach() return code: %d\n", iret);
    exit(EXIT_FAILURE);
  }

  printf("主线程等待5秒后结束...\n");
  sleep(5);

  printf("主线程即将退出...\n");
  exit(EXIT_SUCCESS);
}