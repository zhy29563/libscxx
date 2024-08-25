#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include "Logger.h"

/***********************************************************************************************************************
 * 除以下几点外，ZMQ_XSUB与ZMQ_SUB基本相同
 * 1. 接收消息前需要发送订阅消息
 * 2. 无需设置ZMQ_SUBSCRIBE选项
 * 3. 不会丢失第一条消息
 **********************************************************************************************************************/

void send_subscription_unsubscription_message(void *socket_ptr, bool is_subscription) {
  zmq_msg_t message;
  if (-1 == zmq_msg_init_size(&message, 1)) {
    LOG_ERR("zmq_msg_init_size is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    return;
  }

  uint8_t message_type = is_subscription ? 1 : 0;
  memcpy(zmq_msg_data(&message), &message_type, 1);

  if (-1 == zmq_msg_send(&message, socket_ptr, 0)) {
    LOG_ERR("zmq_msg_send is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    zmq_msg_close(&message);
    return;
  }

  if (-1 == zmq_msg_close(&message)) {
    LOG_ERR("zmq_msg_init_size is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    return;
  }

  LOG_DEB("send %s message is successful.", is_subscription ? "subscription" : "unsubscription");
  zmq_sleep(1);
}
int main() {
  void       *context_ptr = nullptr;
  void       *socket_ptr  = nullptr;
  const char *socket_url  = "tcp://localhost:5555";
  char        recv_buffer[32];
  int         loop_index = 0;

  context_ptr = zmq_ctx_new();
  if (context_ptr == nullptr) {
    LOG_DEB("create zmq context is failed.");
    goto LABEL_EXIT;
  }

  socket_ptr = zmq_socket(context_ptr, ZMQ_XSUB);
  if (socket_ptr == nullptr) {
    LOG_DEB("create zmq socket is failed.");
    goto LABEL_EXIT;
  }

  if (-1 == zmq_connect(socket_ptr, socket_url)) {
    LOG_ERR("zmq_connect is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }

  // 发送订阅消息
  send_subscription_unsubscription_message(socket_ptr, true);

  while (true) {
    (void)memset(recv_buffer, 0, sizeof(recv_buffer) / sizeof(recv_buffer[0]));
    if (-1 == zmq_recv(socket_ptr, recv_buffer, sizeof(recv_buffer) / sizeof(recv_buffer[0]), 0)) {
      LOG_ERR("zmq_recv is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
      goto LABEL_EXIT;
    }

    LOG_DEB("RECV: index = %d, message = %s", loop_index++, recv_buffer);
    if (0 == strcmp(recv_buffer, "exit")) {
      break;
    }
  }

  // 发送取消订阅消息
  send_subscription_unsubscription_message(socket_ptr, true);

LABEL_EXIT:
  if (socket_ptr != nullptr) {
    zmq_close(socket_ptr);
  }

  if (context_ptr != nullptr) {
    if (-1 == zmq_ctx_shutdown(context_ptr)) {
      LOG_ERR("zmq_ctx_shutdown is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    }

    if (-1 == zmq_ctx_term(context_ptr)) {
      LOG_ERR("zmq_ctx_term is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    }
  }

  return EXIT_SUCCESS;
}
