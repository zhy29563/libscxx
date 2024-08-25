#include <zmq.h>

#include <cstdlib>
#include <cstring>

#include "Logger.h"

int main(void) {
  void       *context_ptr = nullptr;
  void       *socket_ptr  = nullptr;
  const char *socket_url  = "tcp://localhost:5555";
  char        recv_buffer[32];

  context_ptr = zmq_ctx_new();
  if (context_ptr == nullptr) {
    LOG_DEB("create zmq context is failed.");
    goto LABEL_EXIT;
  }

  socket_ptr = zmq_socket(context_ptr, ZMQ_REQ);
  if (socket_ptr == nullptr) {
    LOG_DEB("create zmq socket is failed.");
    goto LABEL_EXIT;
  }

  if (-1 == zmq_connect(socket_ptr, socket_url)) {
    LOG_ERR("zmq_connect is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }

  (void)memset(recv_buffer, 0, sizeof(recv_buffer) / sizeof(recv_buffer[0]));
  strcpy(recv_buffer, "hello zmq");
  if (-1 == zmq_send(socket_ptr, recv_buffer, sizeof(recv_buffer) / sizeof(recv_buffer[0]), 0)) {
    LOG_ERR("zmq_send is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }
  LOG_DEB("SEND: message = %s", recv_buffer);

  (void)memset(recv_buffer, 0, sizeof(recv_buffer) / sizeof(recv_buffer[0]));
  if (-1 == zmq_recv(socket_ptr, recv_buffer, sizeof(recv_buffer) / sizeof(recv_buffer[0]), 0)) {
    LOG_ERR("zmq_recv is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }
  LOG_DEB("RECV: message = %s", recv_buffer);

LABEL_EXIT:
  if (socket_ptr != nullptr) {
    zmq_close(socket_ptr);
  }

  if (context_ptr != nullptr) {
    // 关闭上下文导致以下结果
    // 1. 所有从该上下文创建的套接字失效
    // 2. 所有阻塞操作被释放，且errno被设置为ETERM
    // 3. 不能创建新的套接字
    // 4. 仍需要调用zmq_ctx_term进行资源释放
    if (-1 == zmq_ctx_shutdown(context_ptr)) {
      LOG_ERR("zmq_ctx_shutdown is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    }

    if (-1 == zmq_ctx_term(context_ptr)) {
      LOG_ERR("zmq_ctx_term is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    }
  }

  return EXIT_SUCCESS;
}
