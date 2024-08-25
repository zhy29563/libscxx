#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include <string>

static void* ctx = ZMQ_NULL;
static int   ret;

int main(int argc, char const* argv[]) {
  ctx = zmq_ctx_new();
  if (ctx == ZMQ_NULL) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    return EXIT_FAILURE;
  }

  void* server = zmq_socket(ctx, ZMQ_STREAM);
  if (server == ZMQ_NULL) {
    printf("zmq_socket: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  if ((ret = zmq_bind(server, "tcp://*:5001")) != 0) {
    printf("zmq_bind: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  uint8_t     routing_id[255];  // 最小1字节，最大255字节
  size_t      routing_id_size = sizeof(routing_id);
  std::string str_routing_id;

  uint8_t* buffer = nullptr;
  for (int loop = 0; loop < 10; loop++) {
    {
      // 接收路由ID
      zmq_msg_t msg_routing_id;
      zmq_msg_init(&msg_routing_id);
      int msg_len = zmq_msg_recv(&msg_routing_id, server, 0);
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
      msg_len = zmq_msg_recv(&msg_data, server, 0);
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

    // 发送数据前必须获取ZMQ_ROUTING_ID
    if ((ret = zmq_getsockopt(server, ZMQ_ROUTING_ID, routing_id, &routing_id_size)) == -1) {
      printf("zmq_getsockopt: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
    } else {
      printf("=====================================================================================================\n");
      for (int i = 0; i < routing_id_size; i++) {
        str_routing_id += std::to_string(routing_id[i]);
      }
      printf("routing id length = %lu, routing is = %s\n", routing_id_size, str_routing_id.c_str());

      // 发送路由ID
      ret = zmq_send(server, routing_id, routing_id_size, ZMQ_SNDMORE);
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
      ret = zmq_send(server, buffer, strlen(buffer), ZMQ_SNDMORE);
      if (ret == -1) {
        printf("zmq_send   -> data: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
      } else {
        printf("zmq_send   -> data: loop = %d, message length = %5d, message = %s\n", loop, ret, buffer);
      }
    }
  }

  {
    // 关闭指定连接
    {
      printf("=====================================================================================================\n");
      // 发送路由ID
      ret = zmq_send(server, routing_id, routing_id_size, ZMQ_SNDMORE);
      if (ret == -1) {
        printf("zmq_send   -> rout: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
      } else {
        str_routing_id.clear();
        for (int i = 0; i < routing_id_size; i++) {
          str_routing_id += std::to_string(routing_id[i]);
        }
        printf("zmq_send   -> rout: message length = %5d, message = %s\n", ret, str_routing_id.c_str());
      }

      // 发送断开信号
      ret = zmq_send(server, 0, 0, 0);
      if (ret == -1) {
        printf("zmq_send   -> data: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
      } else {
        printf("zmq_send   -> data: message length = %5d\n", ret);
      }
    }
  }

  ret = zmq_close(server);
  if (ret != 0) {
    printf("zmq_close: errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }

  ret = zmq_ctx_term(ctx);
  if (ret != 0) {
    printf("errno = %d, error message = %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
  }
  return 0;
}
