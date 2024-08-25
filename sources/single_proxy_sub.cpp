#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

int main() {
  void *context    = zmq_ctx_new();
  void *subscriber = zmq_socket(context, ZMQ_SUB);

  zmq_connect(subscriber, "tcp://localhost:6001");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "A", 1);
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "B", 1);

  for (size_t count = 0; count < 10; count++) {
    char string[100]{0};
    int  size = zmq_recv(subscriber, string, 100, 0);
    if (size == -1) {
      break;
    }
    string[size] = '\n';
    printf("sub string is %s\n", string);
  }

  zmq_close(subscriber);
  zmq_ctx_term(context);
  return 0;
}
