#include <zmq.h>

#include <cstdlib>
#include <cstring>

#include "Logger.h"

/***********************************************************************************************************************
 * 除以下不同外，ZMQ_XPUB的功能与ZMQ_PUB基本相同：
 * 1. 可以接收订阅消息。消息的大小为一个字节，值为1时表示订阅，值为0时表示取消订阅
 **********************************************************************************************************************/
void recv_subscription_unsubscription_message(void *socket_ptr) {
  zmq_msg_t message;
  if (-1 == zmq_msg_init(&message)) {
    LOG_ERR("zmq_msg_init is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    return;
  }

  if (-1 == zmq_msg_recv(&message, socket_ptr, 0)) {
    LOG_ERR("zmq_msg_recv is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    zmq_msg_close(&message);
    return;
  }

  uint8_t message_type = 255;  // 订阅：1，取消订阅：0。为避免误判，初始值设置为其他值
  memcpy(&message_type, zmq_msg_data(&message), sizeof(message_type));
  if (message_type == 0) {
    LOG_DEB("recv unsubscription message is successful.");
  } else if (message_type == 1) {
    LOG_DEB("recv unsubscription message is successful.");
  } else {
    LOG_DEB("unknown, message_type = %hhd.", message_type);
  }

  if (-1 == zmq_msg_close(&message)) {
    LOG_ERR("zmq_msg_close is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    return;
  }
  zmq_sleep(1);
}

int main(void) {
  void       *context_ptr = nullptr;
  void       *socket_ptr  = nullptr;
  const char *socket_url  = "tcp://*:5555";
  char        recv_buffer[32];
  int         loop_count = 5;

  context_ptr = zmq_ctx_new();
  if (context_ptr == nullptr) {
    LOG_DEB("create zmq context is failed.");
    goto LABEL_EXIT;
  }

  socket_ptr = zmq_socket(context_ptr, ZMQ_XPUB);
  if (socket_ptr == nullptr) {
    LOG_DEB("create zmq socket is failed.");
    goto LABEL_EXIT;
  }

  if (-1 == zmq_bind(socket_ptr, socket_url)) {
    LOG_ERR("zmq_bind is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }

  // 接收订阅消息
  recv_subscription_unsubscription_message(socket_ptr);

  for (int index = 0; index < loop_count; index++) {
    (void)memset(recv_buffer, 0, sizeof(recv_buffer) / sizeof(recv_buffer[0]));
    if (index == loop_count - 1) {
      (void)sprintf(recv_buffer, "exit");
    } else {
      (void)sprintf(recv_buffer, "%d hello zmq", index);
    }

    if (-1 == zmq_send(socket_ptr, recv_buffer, sizeof(recv_buffer) / sizeof(recv_buffer[0]), 0)) {
      LOG_ERR("zmq_send is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
      goto LABEL_EXIT;
    }
    LOG_DEB("SEND: index = %d, message = %s", index, recv_buffer);
    zmq_sleep(1);
  }

  // 接收取消订阅消息
  recv_subscription_unsubscription_message(socket_ptr);

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
  return 0;
}
