#include <zmq.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>

int main(void) {
  void *context   = zmq_ctx_new();
  void *requester = zmq_socket(context, ZMQ_REQ);
  zmq_connect(requester, "tcp://localhost:5555");

  for (int loop = 0; loop < 10; loop++) {
    std::string str = (loop != 9) ? "TEST" : "exit";

    zmq_msg_t msg;
    zmq_msg_init_size(&msg, str.size());
    memcpy(zmq_msg_data(&msg), str.data(), str.size());

    if (zmq_msg_send(&msg, requester, ZMQ_DONTWAIT) == -1) {
      printf("Send data failed.");
    }
    zmq_msg_close(&msg);

    printf("SEND: loop = %d, message = %s\n", loop, str.c_str());

    char buffer[32]{0};
    zmq_recv(requester, buffer, 32, 0);
    printf("RECV: loop = %d, message = %s\n", loop, buffer);

    zmq_sleep(1);
  }

  zmq_close(requester);
  zmq_ctx_destroy(context);
  return 0;
}