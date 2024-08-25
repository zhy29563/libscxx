#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include "Logger.h"

// 关于PUB-SUB套接字还有一件更重要的事情需要了解：你不知道订阅者开始得到消息的精确时间。即使你
// 启动一个订阅者，稍等片刻，然后再启动发布者，订阅者也总是会错过发布者发送的第一个消息。这是因
// 为当订阅者连接到发布者时（这需要的时间很短，但非零），发布者可能已经将消息发送出去了。

/***********************************************************************************************************************
 * ZMQ_SUB用于订阅发布者发布的消息。
 *    初始状态ZMQ_SUB不订阅任何消息，如果需要订阅消息以ZMQ_SUBSCRIBE选项调用zmq_setsockopt函数。
 **********************************************************************************************************************/
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

  socket_ptr = zmq_socket(context_ptr, ZMQ_SUB);
  if (socket_ptr == nullptr) {
    LOG_DEB("create zmq socket is failed.");
    goto LABEL_EXIT;
  }

  // ZMQ_SUB 套接字默认过滤所消息
  // 因此，如果需要接收数据，则需要设置合适的过滤器
  // 过滤规则为：接收消息的开始数据与指定的过滤器匹配的消息
  // 可对一个套接字设置多个过滤器，则满足任何一个的消息都被接收
  // 如果需要接收所有消息，则设置过滤器为空
  // http://api.zeromq.org/master:zmq-setsockopt 搜索 ZMQ_SUBSCRIBE 选项
  if (-1 == zmq_setsockopt(socket_ptr, ZMQ_SUBSCRIBE, "", 0)) {
    LOG_ERR("zmq_setsockopt is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }

  if (-1 == zmq_connect(socket_ptr, socket_url)) {
    LOG_ERR("zmq_connect is failed. error code:%d, error string:%s", zmq_errno(), zmq_strerror(zmq_errno()));
    goto LABEL_EXIT;
  }

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
