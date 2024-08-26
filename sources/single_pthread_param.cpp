#include <pthread.h>  // pthread_create pthread_t
#include <stdlib.h>   // EXIT_FAILURE EXIT_SUCCESS
#include <string.h>   // bzero
#include <unistd.h>   // sleep

#include "Logger.h"

typedef struct student_ {
  int  id;
  int  age;
  char name[32];
} student;

void *start_routine([[maybe_unused]] void *ptr) {
  student *stu_ptr = static_cast<student *>(ptr);
  LOG_DEB("name = %s, age = %d, id = %d", stu_ptr->name, stu_ptr->age, stu_ptr->id);
  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  int       error_code = 0;
  pthread_t thread_id;

  student stu;
  memset(&stu, 0, sizeof(stu));

  strcpy(stu.name, "zao san");
  stu.age = 18;
  stu.id  = 2022;

  error_code = pthread_create(&thread_id, NULL, start_routine, static_cast<void *>(&stu));
  if (0 != error_code) {
    LOG_DEB("pthread_create() return code: %d", error_code);
    return EXIT_FAILURE;
  }

  LOG_DEB("主线程等待5秒后结束...");
  sleep(5);

  LOG_DEB("主线程即将退出...");
  return EXIT_SUCCESS;
}
