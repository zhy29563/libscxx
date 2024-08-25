#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

static int ret;

int get_ctx_options(void* ctx) {
  ret = zmq_ctx_get(ctx, ZMQ_IO_THREADS);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_IO_THREADS = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_MAX_SOCKETS);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_MAX_SOCKETS = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_MAX_MSGSZ);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_MAX_MSGSZ = %d\n", ret);
  }

  // 正式版本中未提供 ZMQ_ZERO_COPY_RECV 选项
  // ret = zmq_ctx_get(ctx, ZMQ_ZERO_COPY_RECV);
  // if (ret == -1) {
  //   printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  // } else {
  //   printf("ZMQ_ZERO_COPY_RECV = %d\n", ret);
  // }

  ret = zmq_ctx_get(ctx, ZMQ_SOCKET_LIMIT);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_SOCKET_LIMIT = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_IPV6);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_IPV6 = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_BLOCKY);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_BLOCKY = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_THREAD_SCHED_POLICY);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_THREAD_SCHED_POLICY = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_THREAD_NAME_PREFIX);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_THREAD_NAME_PREFIX = %d\n", ret);
  }

  ret = zmq_ctx_get(ctx, ZMQ_MSG_T_SIZE);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("ZMQ_MSG_T_SIZE = %d\n", ret);
  }

  return ret;
}

int main(int argc, char const* argv[]) {
  void* ctx = zmq_ctx_new();
  if (ctx == NULL) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return EXIT_FAILURE;
  }

  ret = get_ctx_options(ctx);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  ret = zmq_ctx_shutdown(ctx);
  if (ret != 0) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  // zmq_ctx_destroy() 已被 zmq_ctx_term()取代
  ret = zmq_ctx_term(ctx);
  if (ret != 0) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }
  return 0;
}
