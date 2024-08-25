#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  {
    int detech_state = 0;
    pthread_attr_getdetachstate(&attr, &detech_state);
    printf("default: detech state   = %s\n", detech_state == PTHREAD_CREATE_DETACHED   ? "PTHREAD_CREATE_DETACHED"
                                             : detech_state == PTHREAD_CREATE_JOINABLE ? "PTHREAD_CREATE_JOINABLE"
                                                                                       : "???");
  }

  {
    size_t guard_size = 0;
    pthread_attr_getguardsize(&attr, &guard_size);
    printf("default: guard size     = %lu\n", guard_size);
  }

  {
    int inherit_sched = 0;
    pthread_attr_getinheritsched(&attr, &inherit_sched);
    printf("default: inherit sched  = %s\n", inherit_sched == PTHREAD_INHERIT_SCHED   ? "PTHREAD_INHERIT_SCHED"
                                             : inherit_sched == PTHREAD_INHERIT_SCHED ? "PTHREAD_INHERIT_SCHED"
                                                                                      : "???");
  }

  {
    struct sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    printf("default: sched_param    = %d\n", param.sched_priority);
  }

  {
    int sched_policy = 0;
    pthread_attr_getschedpolicy(&attr, &sched_policy);
    printf("default: sched policy   = %s\n", sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
                                             : sched_policy == SCHED_RR    ? "SCHED_RR"
                                             : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                                                           : "???");
  }

  {
    int scope = 0;
    pthread_attr_getscope(&attr, &scope);
    printf("default: scope          = %s\n", scope == PTHREAD_SCOPE_SYSTEM    ? "PTHREAD_SCOPE_SYSTEM"
                                             : scope == PTHREAD_SCOPE_PROCESS ? "PTHREAD_SCOPE_PROCESS"
                                                                              : "???");
  }

  {
    void *stack_addr = NULL;
    size_t stack_size = 0;
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    printf("default: stack_addr     = %p\n", stack_addr);
    printf("default: stack_size     = %lu\n", stack_size);
  }

  {
    void *stack_addr = NULL;
    pthread_attr_getstackaddr(&attr, &stack_addr);  // 已弃用
    printf("default: stack_addr     = %p\n", stack_addr);
  }

  {
    size_t stack_size = 0;
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("default: stack_size     = %lu\n", stack_size);
  }
  pthread_attr_destroy(&attr);
  // pthread_getcpuclockid
  // pthread_getschedparam
  // pthread_getspecific
  // pthread_setschedparam
  // pthread_setschedprio
  // pthread_setspecific
  exit(EXIT_SUCCESS);
}