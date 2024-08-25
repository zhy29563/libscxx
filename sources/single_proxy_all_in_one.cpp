#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

#include <atomic>
#include <thread>

#define randof(num) (int)((float)(num)*random() / (RAND_MAX + 1.0))

void thread_publish(void *context) {
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
}

void thread_subscribe(void *context) {
  void *subscriber = zmq_socket(context, ZMQ_SUB);

  zmq_connect(subscriber, "tcp://localhost:6001");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "A", 1);
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "B", 1);

  int count = 0;
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
}

void thread_proxy(void *context) {
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
}

int main(int argc, char const *argv[]) {
  void       *context = zmq_ctx_new();
  std::thread t1(thread_subscribe, context);
  std::thread t2(thread_proxy, context);
  std::thread t3(thread_publish, context);

  t1.join();
  t2.join();
  t3.join();

  zmq_ctx_term(context);
  return 0;
}
