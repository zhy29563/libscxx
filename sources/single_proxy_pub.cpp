#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

int main() {
  void *context   = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  zmq_bind(publisher, "tcp://*:6000");

  for (size_t count = 0; count < 100; count++) {
    char string[10]{0};
    sprintf(string, "%c-%5d-%lu", randof(10) + 'A', randof(100000), count);  // NOLINT (cert-err33-c)

    if (zmq_send(publisher, string, strlen(string), 0) == -1) {
      break;
    }
    printf("pub string is %s\n", string);

    zmq_sleep(1);
  }

  zmq_close(publisher);
  zmq_ctx_term(context);
  return 0;
}
