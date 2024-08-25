#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void *start_routine(void *ptr) {
  printf("子线程(%lu)等待3秒后退出...\n", pthread_self());

  clockid_t clock_id;
  pthread_getcpuclockid(pthread_self(), &clock_id);
  printf("子线程(%lu)的CPU时钟ID(%d)\n", pthread_self(), clock_id);

  int sched_policy;
  struct sched_param param;
  pthread_getschedparam(pthread_self(), &sched_policy, &param);
  printf("子线程(%lu)的调度策略(%s)\n", pthread_self(),
         sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
         : sched_policy == SCHED_RR    ? "SCHED_RR"
         : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                       : "???");
  printf("子线程(%lu)的调度优先级(%d)\n", pthread_self(), param.sched_priority);

  printf("子线程(%lu)即将退出...\n", pthread_self());
  return (void *)NULL;
}

int main(int argc, char const *argv[]) {
  printf("主线程(%lu)开始运行...\n", pthread_self());

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, start_routine, NULL);

  clockid_t clock_id;
  pthread_getcpuclockid(pthread_self(), &clock_id);
  printf("主线程(%lu)的CPU时钟ID(%d)\n", pthread_self(), clock_id);

  int sched_policy;
  struct sched_param param;
  pthread_getschedparam(pthread_self(), &sched_policy, &param);
  printf("主线程(%lu)的调度策略(%s)\n", pthread_self(),
         sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
         : sched_policy == SCHED_RR    ? "SCHED_RR"
         : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                       : "???");
  printf("主线程(%lu)的调度优先级(%d)\n", pthread_self(), param.sched_priority);
  printf("主线程(%lu)等待5秒后结束...\n", pthread_self());
  sleep(5);

  printf("主线程(%lu)即将退出...\n", pthread_self());
  exit(EXIT_SUCCESS);
}