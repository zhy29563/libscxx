#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

// PUB("tcp:://*:6000")  XSUB("tcp:://localhost:6000")
//                       PROXY
//                       XPUB("tcp://*:6001")
//                                                  SUB("tcp:://localhost:6001")

int main() {
  void *context = zmq_ctx_new();

  void *subscriber = zmq_socket(context, ZMQ_XSUB);
  zmq_connect(subscriber, "tcp://localhost:6000");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

  void *publisher = zmq_socket(context, ZMQ_XPUB);
  zmq_bind(publisher, "tcp://*:6001");

  // 发送订阅
  {
    uint8_t sub_type = 1;
    zmq_send(subscriber, &sub_type, 1, 0);
  }

  for (size_t count = 0; count < 80; count++) {
    // 订阅
    char string[100]{0};
    {
      int size = zmq_recv(subscriber, string, 100, 0);
      if (size == -1) {
        printf("break.\n");
        break;
      }
      string[size] = '\0';
      printf("proxy string is %s\n", string);
    }

    // 发送
    zmq_send(publisher, string, 100, 0);
  }

  zmq_close(publisher);
  zmq_close(subscriber);
  zmq_ctx_term(context);
  return 0;
}
