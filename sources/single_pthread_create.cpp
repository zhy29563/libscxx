#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "Logger.h"

void *start_routine([[maybe_unused]] void *ptr) {
  LOG_DEB("子线程(%lu)等待3秒后退出...", pthread_self());

  clockid_t clock_id;
  pthread_getcpuclockid(pthread_self(), &clock_id);
  LOG_DEB("子线程(%lu)的CPU时钟ID(%d)", pthread_self(), clock_id);

  int                sched_policy;
  struct sched_param param;
  pthread_getschedparam(pthread_self(), &sched_policy, &param);
  LOG_DEB("子线程(%lu)的调度策略(%s)", pthread_self(),
          sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
          : sched_policy == SCHED_RR    ? "SCHED_RR"
          : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                        : "???");
  LOG_DEB("子线程(%lu)的调度优先级(%d)", pthread_self(), param.sched_priority);
  LOG_DEB("子线程(%lu)即将退出...", pthread_self());

  return nullptr;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  LOG_DEB("主线程(%lu)开始运行...", pthread_self());

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, start_routine, NULL);

  clockid_t clock_id;
  pthread_getcpuclockid(pthread_self(), &clock_id);
  LOG_DEB("主线程(%lu)的CPU时钟ID(%d)", pthread_self(), clock_id);

  int                sched_policy;
  struct sched_param param;
  pthread_getschedparam(pthread_self(), &sched_policy, &param);
  LOG_DEB("主线程(%lu)的调度策略(%s)", pthread_self(),
          sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
          : sched_policy == SCHED_RR    ? "SCHED_RR"
          : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                        : "???");
  LOG_DEB("主线程(%lu)的调度优先级(%d)", pthread_self(), param.sched_priority);
  LOG_DEB("主线程(%lu)等待5秒后结束...", pthread_self());
  sleep(5);

  LOG_DEB("主线程(%lu)即将退出...", pthread_self());
  return EXIT_SUCCESS;
}
