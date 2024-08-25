#include <stdio.h>
#include <zmq.h>

#include <thread>

void thread_request(void* context) {
  void* requester = zmq_socket(context, ZMQ_REQ);
  zmq_connect(requester, "tcp://localhost:5559");

  char buffer[256];
  for (size_t i = 0; i < 5; i++) {
    printf("==================================================================\n");
    int size = zmq_send(requester, "Hello", 5, 0);
    printf("req  ->send:index = %lu, payload = %s\n", i, "Hello");

    size                            = zmq_recv(requester, buffer, 256, 0);
    buffer[size > 256 ? 255 : size] = '\0';
    printf("req  ->recv:index = %lu, payload = %s\n", i, buffer);

    zmq_sleep(1);
  }

  zmq_close(requester);
}

void thread_response(void* context) {
  void* response = zmq_socket(context, ZMQ_REP);
  zmq_connect(response, "tcp://localhost:5560");

  char buffer[256];
  for (size_t i = 0; i < 5; i++) {
    int size                        = zmq_recv(response, buffer, 256, 0);
    buffer[size > 256 ? 255 : size] = '\0';
    printf("rep  ->recv:index = %lu, payload = %s\n", i, buffer);

    size = zmq_send(response, "World", 5, 0);
    printf("rep  ->send:index = %lu, payload = %s\n", i, "World");
  }

  zmq_close(response);
}

void thread_proxy(void* context) {
  void* frontend = zmq_socket(context, ZMQ_ROUTER);
  void* backend  = zmq_socket(context, ZMQ_DEALER);

  zmq_bind(frontend, "tcp://*:5559");
  zmq_bind(backend, "tcp://*:5560");

  zmq_pollitem_t items[] = {{frontend, 0, ZMQ_POLLIN, 0}, {backend, 0, ZMQ_POLLIN, 0}};

  for (size_t i = 0; i < 10; i++) {
    zmq_msg_t msg;
    int       more;

    zmq_poll(items, 2, -1);

    // req->rep
    if ((items[0].revents & ZMQ_POLLIN) != 0) {
      while (true) {
        zmq_msg_init(&msg);
        int size = zmq_msg_recv(&msg, frontend, 0);
        printf("proxy->recv[req->rep], index = %lu, size = %d\n", i, size);

        size_t more_size = sizeof(more);
        zmq_getsockopt(frontend, ZMQ_RCVMORE, &more, &more_size);
        size = zmq_msg_send(&msg, backend, more != 0 ? ZMQ_SNDMORE : 0);
        printf("proxy->send[req->rep], index = %lu, size = %d\n", i, size);

        zmq_msg_close(&msg);
        if (more == 0) {
          break;
        }
      }
    }

    // rep->req
    if ((items[1].revents & ZMQ_POLLIN) != 0) {
      while (true) {
        zmq_msg_init(&msg);
        int size = zmq_msg_recv(&msg, backend, 0);
        printf("proxy->recv[rep->req], index = %lu, size = %d\n", i, size);

        size_t more_size = sizeof(more);
        zmq_getsockopt(backend, ZMQ_RCVMORE, &more, &more_size);
        size = zmq_msg_send(&msg, frontend, more != 0 ? ZMQ_SNDMORE : 0);
        printf("proxy->send[rep->req], index = %lu, size = %d\n", i, size);

        zmq_msg_close(&msg);
        if (more == 0) {
          break;
        }
      }
    }
  }

  zmq_close(frontend);
  zmq_close(backend);
}

int main(int argc, char const* argv[]) {
  void* context = zmq_ctx_new();

  std::thread t1(thread_response, context);
  std::thread t2(thread_proxy, context);
  std::thread t3(thread_request, context);

  t1.join();
  t2.join();
  t3.join();

  zmq_ctx_destroy(context);
  return 0;
}
