#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

static void* ctx = ZMQ_NULL;
static int   ret;

void* thread_one(void*) {
  void* req = zmq_socket(ctx, ZMQ_PAIR);
  if (req == ZMQ_NULL) {
    printf("thread_one->zmq_socket: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return static_cast<void*>(0);
  }

  ret = zmq_bind(req, "inproc://my-endpoint");
  if (ret != 0) {
    printf("thread_one->zmq_bind: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  for (int loop = 0; loop < 10; loop++) {
    char msg[256];
    memset(msg, 0, 256);  // return value: A copy of dest
    if (loop != 9) {
      // return value:
      // number of characters (not including the terminating null character) which would have been written to buffer if
      // bufsz was ignored, or a negative value if an encoding error (for string and character conversion specifiers)
      // occurred
      ret = snprintf(msg, 256, "SEND: index = %d", loop);
      // printf("Number of bytes writed to buffer = %d\n", ret);
    } else {
      strcpy(msg, "exit");  // returns a copy of dest
    }

    // strlen 返回不包含结尾的\0
    ret = zmq_send(req, msg, strlen(msg), 0);
    if (ret == -1) {
      printf("thread_one->zmq_send: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    } else {
      printf("SEND: loop = %d, message length = %d, message = %s\n", loop, ret, msg);
    }

    zmq_sleep(1);
  }

  ret = zmq_close(req);
  if (ret != 0) {
    printf("thread_one->zmq_close: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  return ctx;
}

void* thread_two(void*) {
  void* rep = zmq_socket(ctx, ZMQ_PAIR);
  if (rep == ZMQ_NULL) {
    printf("thread_two->zmq_socket: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return static_cast<void*>(0);
  }

  ret = zmq_connect(rep, "inproc://my-endpoint");
  if (ret != 0) {
    printf("thread_two->zmq_connect: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  for (int loop = 0; loop < 10; loop++) {
    char buffer[256];
    memset(buffer, 0, 256);

    ret = zmq_recv(rep, buffer, 32, 0);
    if (ret == -1) {
      printf("thread_two->zmq_recv: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    } else {
      printf("RECV: loop = %d, message length = %d, message = %s\n", loop, ret, buffer);
    }
  }

  ret = zmq_close(rep);
  if (ret != 0) {
    printf("thread_two->zmq_close: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }
  return ctx;
}

int main(int argc, char const* argv[]) {
  ctx = zmq_ctx_new();
  if (ctx == ZMQ_NULL) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return EXIT_FAILURE;
  }

  pthread_t t1;
  pthread_t t2;
  pthread_create(&t1, NULL, thread_one, ZMQ_NULL);
  pthread_create(&t2, NULL, thread_two, ZMQ_NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  ret = zmq_ctx_term(ctx);
  if (ret != 0) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }
  return 0;
}
