#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int exit_flag = 0;

void *start_routine(void *ptr) {
  do {
    printf("++++++++++++++++++++++++++\n");
    usleep(500000);
  } while (!exit_flag);

  return (void *)"9999";
}

void exit_func() {
  printf("------------------------------------\n");
  exit_flag = 1;
  usleep(2000000);
  printf("====================================\n");
}

int main(int argc, char const *argv[]) {
  int a = atexit(exit_func);
  printf("atexit return a = %d\n", a);

  // int iret = 0;
  // pthread_t thread_id;
  // pthread_create(&thread_id, NULL, start_routine, NULL);
  // pthread_detach(thread_id);
  // void *retval = NULL;
  // iret = pthread_join(thread_id, &retval);
  // if (iret)
  // {
  //     fprintf(stderr, "Error - pthread_join() return code: %d\n", iret);
  //     exit(EXIT_FAILURE);
  // }
  // printf("子线程的返回值为：%s\n", (const char *)retval);
  // do
  // {
  //     printf("*********************\n");
  //     usleep(500000);
  // } while (!exit_flag);

  printf("主线程(%lu)即将退出...\n", pthread_self());
  exit(EXIT_SUCCESS);
}