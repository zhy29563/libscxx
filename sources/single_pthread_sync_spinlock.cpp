#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_spinlock_t spinlock;

void *start_routine_01(void *ptr) {
  printf("子线程(%lu)开始运行...\n", pthread_self());
  for (size_t i = 0; i < 3; i++) {
    pthread_spin_lock(&spinlock);
    printf("子线程(%lu)进入自旋锁\n", pthread_self());
    sleep(1);
    printf("子线程(%lu)离开自旋锁\n", pthread_self());
    pthread_spin_unlock(&spinlock);
  }
  return (void *)"9999";
}

void *start_routine_02(void *ptr) {
  printf("子线程(%lu)开始运行...\n", pthread_self());
  for (size_t i = 0; i < 3; i++) {
    pthread_spin_lock(&spinlock);
    printf("子线程(%lu)进入自旋锁\n", pthread_self());
    sleep(1);
    printf("子线程(%lu)离开自旋锁\n", pthread_self());
    pthread_spin_unlock(&spinlock);
  }
  return (void *)"9999";
}

int main(int argc, char const *argv[]) {
  printf("主线程(%lu)开始运行...\n", pthread_self());

  pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

  pthread_t thread_id_01;
  pthread_create(&thread_id_01, NULL, start_routine_01, NULL);

  pthread_t thread_id_02;
  pthread_create(&thread_id_02, NULL, start_routine_02, NULL);

  pthread_join(thread_id_01, NULL);
  pthread_join(thread_id_02, NULL);

  pthread_spin_destroy(&spinlock);
  printf("主线程(%lu)即将退出...\n", pthread_self());
  exit(EXIT_SUCCESS);
}