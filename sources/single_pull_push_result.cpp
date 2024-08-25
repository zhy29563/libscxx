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

#include <chrono>

int main(void) {
  void *context  = zmq_ctx_new();
  void *receiver = zmq_socket(context, ZMQ_PULL);
  zmq_bind(receiver, "tcp://*:5558");  // 接收启动命令与任务结果

  // 接收启动命令
  zmq_msg_t msg;
  zmq_msg_init(&msg);
  zmq_msg_recv(&msg, receiver, 0);
  zmq_msg_close(&msg);

  auto start_time = std::chrono::system_clock::now();
  for (int num = 0; num < 100; num++) {
    zmq_msg_init(&msg);
    zmq_msg_recv(&msg, receiver, 0);
    zmq_msg_close(&msg);

    if (num % 10 == 0) {
      puts(":");
    } else {
      puts(".");
    }
  }
  //  Calculate and report duration of batch
  auto end_time = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  printf("Total elapsed time: %ld msec\n", duration.count());

  zmq_close(receiver);
  zmq_ctx_destroy(context);
  return 0;
}