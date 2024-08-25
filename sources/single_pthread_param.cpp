#include <pthread.h>  // pthread_create pthread_t
#include <stdio.h>    // printf fprintf
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <string.h>   // bzero
#include <unistd.h>   // sleep

typedef struct _student {
  int id;
  int age;
  char name[32];
} student;

void *start_routine(void *ptr) {
  student *pstu = (student *)ptr;
  printf("name = %s, age = %d, id = %d\n", pstu->name, pstu->age, pstu->id);
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  int iret = 0;
  pthread_t thread_id;

  student stu;
  bzero(&stu, sizeof(stu));

  strcpy(stu.name, "zhang san");
  stu.age = 18;
  stu.id = 2022;

  iret = pthread_create(&thread_id, NULL, start_routine, (void *)&stu);
  if (iret) {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
    exit(EXIT_FAILURE);
  }

  printf("主线程等待5秒后结束...\n");
  sleep(5);

  printf("主线程即将退出...\n");
  exit(EXIT_SUCCESS);
}