#include <pthread.h>  // pthread_create pthread_t
#include <stdio.h>    // printf fprintf
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <unistd.h>   // sleep

void *start_routine(void *ptr) {
  printf("子线程等待3秒后退出...\n");
  sleep(3);
  return (void *)"9999";
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

  void *retval = NULL;
  iret = pthread_join(thread_id, &retval);
  if (iret) {
    fprintf(stderr, "Error - pthread_join() return code: %d\n", iret);
    exit(EXIT_FAILURE);
  }
  printf("子线程的返回值为：%s\n", (const char *)retval);

  printf("主线程即将退出...\n");
  exit(EXIT_SUCCESS);
}