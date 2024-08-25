#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *start_routine_01(void *ptr) {
  printf("进入阻塞状态...\n");
  pthread_mutex_lock(&mutex);
  pthread_cond_wait(&cond, &mutex);
  printf("获得信号以解除阻塞状态...\n");

  pthread_mutex_unlock(&mutex);
  return (void *)NULL;
}

void *start_routine_02(void *ptr) {
  printf("等待5秒后发出解除信号...\n");
  sleep(5);
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cond);
  printf("解除信号已发出...\n");

  pthread_mutex_unlock(&mutex);
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  {
    pthread_mutexattr_t attr;       // 定义互斥属性
    pthread_mutexattr_init(&attr);  // 初始化互斥属性

    pthread_mutex_init(&mutex, &attr);  // 初始化互斥
    pthread_mutexattr_destroy(&attr);   // 销毁互斥属性
  }

  {
    pthread_condattr_t attr;       // 定义条件变量属性
    pthread_condattr_init(&attr);  // 初始化条件变量属性

    pthread_cond_init(&cond, &attr);  // 初始化条件变量
    pthread_condattr_destroy(&attr);  // 销毁条件变量属性
  }

  pthread_t thread_id_01;
  pthread_t thread_id_02;

  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_01, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  pthread_mutex_destroy(&mutex);  // 销毁互斥
  pthread_cond_destroy(&cond);    // 销毁条件变量

  exit(EXIT_SUCCESS);
}