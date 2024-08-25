// 原生模式
//  支持TCP传输的服务器与客户端的双向异步通讯
//
// ZMQ_STREAM
//  支持与非ZMQ的TCP客户端或服务器的异步双向通讯。
//  该类型的套接字既可以作为服务器，亦可以作为客户端
//
//  从非ZMQ接收消息时，该套接字会在数据前填充一个称为路由标识的消息
//  发送消息到非ZMQ时，该套接字会先发送一个称为路由标识的消息
//
//  为了发送数据，需要调用zmq_getsockopt，获取ZMQ_ROUTING_ID选项的值，以便获取消息标识
//  关闭连接时，需要先发送消息标识，然后发送一个长度为零的消息
//
//  连接建立与断开时，均会收到一个消息长度为零的消息

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include <string>

static void*       ctx = ZMQ_NULL;
static int         ret;
static int         connected = 0;
static const char* address   = "tcp://127.0.0.1:8080";

int main(int argc, char const* argv[]) {
  ctx = zmq_ctx_new();
  if (ctx == ZMQ_NULL) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return EXIT_FAILURE;
  }

  void* client = zmq_socket(ctx, ZMQ_STREAM);
  if (client == ZMQ_NULL) {
    printf("zmq_socket: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  ret = zmq_connect(client, address);
  if (ret == -1) {
    printf("zmq_connect: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  uint8_t routing_id[255];  // 最小1字节，最大255字节
  size_t  routing_id_size = sizeof(routing_id);
  ret                     = zmq_getsockopt(client, ZMQ_ROUTING_ID, routing_id, &routing_id_size);
  if (ret == -1) {
    printf("zmq_getsockopt: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  } else {
    printf("=======================================================================================================\n");
    std::string str_routing_id;
    for (int i = 0; i < routing_id_size; i++) {
      str_routing_id += std::to_string(routing_id[i]);
    }
    printf("routing id length = %lu, routing is = %s\n", routing_id_size, str_routing_id.c_str());
    printf("=======================================================================================================\n");

    uint8_t* buffer = nullptr;
    for (int loop = 0; loop < 10; loop++) {
      {
        // 接收路由ID
        zmq_msg_t msg_routing_id;
        zmq_msg_init(&msg_routing_id);
        int msg_len = zmq_msg_recv(&msg_routing_id, client, 0);
        if (msg_len == -1) {
          printf("zmq_msg_recv->rout: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
        } else {
          buffer = new uint8_t[msg_len + 1];
          memset(buffer, 0, msg_len + 1);
          memcpy(buffer, zmq_msg_data(&msg_routing_id), msg_len);
          str_routing_id.clear();
          for (int i = 0; i < msg_len; i++) {
            str_routing_id += std::to_string(buffer[i]);
          }

          printf("zmq_msg_recv->rout: loop = %d, message length = %5d, message = %s\n", loop, msg_len,
                 str_routing_id.c_str());
          delete[] buffer;
        }
        zmq_msg_close(&msg_routing_id);

        // 接收数据
        zmq_msg_t msg_data;
        zmq_msg_init(&msg_data);
        msg_len = zmq_msg_recv(&msg_data, client, 0);
        if (msg_len == -1) {
          printf("zmq_msg_recv->data: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
        } else {
          buffer = new uint8_t[msg_len + 1];
          memset(buffer, 0, msg_len + 1);
          memcpy(buffer, zmq_msg_data(&msg_data), msg_len);
          printf("zmq_msg_recv->data: loop = %d, message length = %5d, message = %s\n", loop, msg_len, buffer);
          delete[] buffer;
        }
        zmq_msg_close(&msg_data);
      }

      {
        // 发送路由ID
        ret = zmq_send(client, routing_id, routing_id_size, ZMQ_SNDMORE);
        if (ret == -1) {
          printf("zmq_send   -> rout: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
        } else {
          str_routing_id.clear();
          for (int i = 0; i < routing_id_size; i++) {
            str_routing_id += std::to_string(routing_id[i]);
          }
          printf("zmq_send   -> rout: loop = %d, message length = %5d, message = %s\n", loop, ret,
                 str_routing_id.c_str());
        }

        // 发送数据
        char buffer[256];
        memset(buffer, 0, 256);
        sprintf(buffer, "Index%d\n", loop);  // NOLINT (cert-err33-c)
        ret = zmq_send(client, buffer, strlen(buffer), ZMQ_SNDMORE);
        if (ret == -1) {
          printf("zmq_send   -> data: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
        } else {
          printf("zmq_send   -> data: loop = %d, message length = %5d, message = %s\n", loop, ret, buffer);
        }
      }
    }
  }

  ret = zmq_disconnect(client, address);
  if (ret == -1) {
    printf("zmq_close: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  ret = zmq_close(client);
  if (ret == -1) {
    printf("zmq_close: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  ret = zmq_ctx_term(ctx);
  if (ret == -1) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  return 0;
}
