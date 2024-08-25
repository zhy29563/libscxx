#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_num = 0;
pthread_rwlock_t rwlock;

void *start_routine_01(void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_rdlock(&rwlock);
    for (size_t i = 0; i < 2; i++) {
      printf("读线程(%lu)获取全局变量当前值(%d)\n", pthread_self(), g_num);
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }

  return (void *)NULL;
}

void *start_routine_02(void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_rdlock(&rwlock);
    for (size_t i = 0; i < 2; i++) {
      printf("读线程(%lu)获取全局变量当前值(%d)\n", pthread_self(), g_num);
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }

  return (void *)NULL;
}

void *start_routine_03(void *ptr) {
  for (size_t i = 0; i < 5; i++) {
    pthread_rwlock_wrlock(&rwlock);
    printf("写线程(%lu)设置全局变量当前值(%d)\n", pthread_self(), ++g_num);
    pthread_rwlock_unlock(&rwlock);
    sleep(2);
  }

  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  pthread_rwlockattr_t attr;
  pthread_rwlockattr_init(&attr);
  pthread_rwlock_init(&rwlock, &attr);
  pthread_rwlockattr_destroy(&attr);

  pthread_t thread_id_01, thread_id_02, thread_id_03;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);
  pthread_create(&thread_id_03, NULL, start_routine_03, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);
  pthread_join(thread_id_03, NULL);

  pthread_rwlock_destroy(&rwlock);
  exit(EXIT_SUCCESS);
}