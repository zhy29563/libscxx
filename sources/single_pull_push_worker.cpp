#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <zmq.h>

int main(void) {
  void *context  = zmq_ctx_new();
  void *receiver = zmq_socket(context, ZMQ_PULL);  // 接收任务
  zmq_connect(receiver, "tcp://localhost:5557");

  void *sender = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(sender, "tcp://localhost:5558");  // 发送结果

  while (true) {
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    int   buffer_size = zmq_msg_recv(&msg, receiver, 0);
    char *buffer      = new char[buffer_size + 1];
    memset(buffer, 0, buffer_size + 1);
    memcpy(buffer, zmq_msg_data(&msg), buffer_size);
    zmq_msg_close(&msg);

    puts(buffer);

    usleep(strtol(buffer, nullptr, 10));  // 任务处理

    delete[] buffer;
    zmq_send(sender, "", 0, 0);
  }
  zmq_close(receiver);
  zmq_close(sender);
  zmq_ctx_destroy(context);

  return EXIT_SUCCESS;
}