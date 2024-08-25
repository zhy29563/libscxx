#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <zmq.h>

// 必须同步开始同批次所有工人的启动和运行。这是ZMQ中存在的一个相当普遍的疑难杂症，并没有简单的方案
// connect 方法需要一定的时间，所以当一组工人连接到任务发生器时，第一个成功连接的工人会在这短短的
// 时间得到消息的整个负载，而其他工人仍在进行连接。如果不知何故批次的开始不同步，那么系统将无法并行运行

#define randof(num) (int)((float)(num)*random() / (RAND_MAX + 1.0))

int main(void) {
  void *context   = zmq_ctx_new();
  void *work_push = zmq_socket(context, ZMQ_PUSH);
  zmq_bind(work_push, "tcp://*:5557");

  void *start_push = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(start_push, "tcp://localhost:5558");

  printf("Press Enter when the workers are ready: ");
  getchar();  // NOLINT (cert-err33-c)

  // 通知任务开始
  zmq_send(start_push, "0", 1, 0);

  //  Initialize random number generator
  srandom(static_cast<unsigned>(time(NULL)));

  //  Send 100 tasks
  int total_msec = 0;  //  Total expected cost in msecs
  for (int task_nbr = 0; task_nbr < 100; task_nbr++) {
    int workload = randof(100) + 1;  //  Random workload from 1 to 100msecs
    total_msec += workload;
    char buffer[10];
    sprintf(buffer, "%d", workload);  // NOLINT (cert-err33-c)
    zmq_send(work_push, buffer, strlen(buffer), 0);
  }
  printf("Total expected cost: %d msec\n", total_msec);

  zmq_close(start_push);
  zmq_close(work_push);
  zmq_ctx_destroy(context);
  return 0;
}