#include <zmq.h>

#include <cstdio>
#include <cstring>

int main(void) {
  void *context = zmq_ctx_new();
  void *responder = zmq_socket(context, ZMQ_REP);
  zmq_bind(responder, "tcp://*:5555");

  int loop = 0;
  while (true) {
    char buffer[32]{0};
    zmq_recv(responder, buffer, 32, 0);
    printf("RECV: loop = %d, message = %s\n", loop, buffer);

    zmq_send(responder, buffer, 32, 0);
    printf("SEND: loop = %d, message = %s\n", loop, buffer);

    zmq_sleep(1);
    if (strcmp("exit", buffer) == 0) {
      break;
    }
    loop++;
  }
  return 0;
}