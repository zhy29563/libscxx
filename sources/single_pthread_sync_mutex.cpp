#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int g_num = 0;

void *start_routine_01(void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    // 锁定最小单元
    pthread_mutex_lock(&mutex);  // 锁定
    g_num++;
    pthread_mutex_unlock(&mutex);  // 解锁
  }
  return (void *)NULL;
}

void *start_routine_02(void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    // 锁定最小单元
    pthread_mutex_lock(&mutex);  // 锁定
    g_num++;
    pthread_mutex_unlock(&mutex);  // 解锁
  }
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  pthread_mutexattr_t attr;       // 定义互斥属性
  pthread_mutexattr_init(&attr);  // 初始化互斥属性

  pthread_mutex_init(&mutex, &attr);  // 初始化互斥
  pthread_mutexattr_destroy(&attr);   // 销毁互斥属性

  pthread_t thread_id_01;
  pthread_t thread_id_02;

  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_01, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  printf("计算结果为： %d\n", g_num);

  printf("主线程即将退出...\n");

  pthread_mutex_destroy(&mutex);  // 销毁互斥
  exit(EXIT_SUCCESS);
}