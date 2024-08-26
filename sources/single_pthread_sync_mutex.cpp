#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "Logger.h"

pthread_mutex_t mutex;
int             g_num = 0;

void *start_routine_01([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    // 锁定最小单元
    pthread_mutex_lock(&mutex);  // 锁定
    g_num++;
    pthread_mutex_unlock(&mutex);  // 解锁
  }
  return nullptr;
}

void *start_routine_02([[maybe_unused]] void *ptr) {
  for (size_t i = 0; i < 10000; i++) {
    // 锁定最小单元
    pthread_mutex_lock(&mutex);  // 锁定
    g_num++;
    pthread_mutex_unlock(&mutex);  // 解锁
  }
  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  pthread_mutexattr_t attr;       // 定义互斥属性
  pthread_mutexattr_init(&attr);  // 初始化互斥属性

  pthread_mutex_init(&mutex, &attr);  // 初始化互斥
  pthread_mutexattr_destroy(&attr);   // 销毁互斥属性

  pthread_t thread_id_01;
  pthread_t thread_id_02;

  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  LOG_DEB("计算结果为： %d", g_num);
  LOG_DEB("主线程即将退出...");

  pthread_mutex_destroy(&mutex);  // 销毁互斥
  return EXIT_SUCCESS;
}
