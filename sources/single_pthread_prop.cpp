#include <pthread.h>
#include <stdlib.h>

#include "Logger.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  {
    int detached_state = 0;
    pthread_attr_getdetachstate(&attr, &detached_state);
    LOG_DEB("default: detached state   = %s", detached_state == PTHREAD_CREATE_DETACHED   ? "PTHREAD_CREATE_DETACHED"
                                              : detached_state == PTHREAD_CREATE_JOINABLE ? "PTHREAD_CREATE_JOINABLE"
                                                                                          : "???");
  }

  {
    size_t guard_size = 0;
    pthread_attr_getguardsize(&attr, &guard_size);
    LOG_DEB("default: guard size     = %lu", guard_size);
  }

  {
    int inherit_sched = 0;
    pthread_attr_getinheritsched(&attr, &inherit_sched);
    LOG_DEB("default: inherit sched  = %s", inherit_sched == PTHREAD_INHERIT_SCHED   ? "PTHREAD_INHERIT_SCHED"
                                            : inherit_sched == PTHREAD_INHERIT_SCHED ? "PTHREAD_INHERIT_SCHED"
                                                                                     : "???");
  }

  {
    struct sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    LOG_DEB("default: sched_param    = %d", param.sched_priority);
  }

  {
    int sched_policy = 0;
    pthread_attr_getschedpolicy(&attr, &sched_policy);
    LOG_DEB("default: sched policy   = %s", sched_policy == SCHED_FIFO    ? "SCHED_FIFO"
                                            : sched_policy == SCHED_RR    ? "SCHED_RR"
                                            : sched_policy == SCHED_OTHER ? "SCHED_OTHER"
                                                                          : "???");
  }

  {
    int scope = 0;
    pthread_attr_getscope(&attr, &scope);
    LOG_DEB("default: scope          = %s", scope == PTHREAD_SCOPE_SYSTEM    ? "PTHREAD_SCOPE_SYSTEM"
                                            : scope == PTHREAD_SCOPE_PROCESS ? "PTHREAD_SCOPE_PROCESS"
                                                                             : "???");
  }

  {
    void  *stack_addr = NULL;
    size_t stack_size = 0;
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    LOG_DEB("default: stack_addr     = %p", stack_addr);
    LOG_DEB("default: stack_size     = %lu", stack_size);
  }

  {
    void  *stack_addr = NULL;
    size_t stack_addr_size;
    pthread_attr_getstack(&attr, &stack_addr, &stack_addr_size);
    LOG_DEB("default: stack_addr     = %p", stack_addr);
  }

  {
    size_t stack_size = 0;
    pthread_attr_getstacksize(&attr, &stack_size);
    LOG_DEB("default: stack_size     = %lu", stack_size);
  }
  pthread_attr_destroy(&attr);
  // pthread_getcpuclockid
  // pthread_getschedparam
  // pthread_getspecific
  // pthread_setschedparam
  // pthread_setschedprio
  // pthread_setspecific
  return EXIT_SUCCESS;
}
