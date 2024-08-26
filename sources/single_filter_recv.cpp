#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/select.h>
#endif

#include <fcntl.h>
#include <lcm/lcm.h>

#include <bitset>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4819)

int test_count     = 0;
int test_count2    = 0;
int catchall_count = 0;

void test_handler(const lcm_recv_buf_t *buf, const char *channel, void *u) {
  printf("HD0 [%s] msg (content: %s)\n", channel, (char *)buf->data);
  test_count++;
}

void test_handler2(const lcm_recv_buf_t *buf, const char *channel, void *u) {
  printf("HD1 [%s] msg (content: %s)\n", channel, (char *)buf->data);
  test_count2++;
}

void catchall_handler(const lcm_recv_buf_t *buf, const char *channel, void *u) {
  printf("ALL [%s] msg (content: %s)\n", channel, (char *)buf->data);
  catchall_count++;
}

int main() {
  char *data     = (char *)"payload";
  int   data_len = strlen(data) + 1;

  lcm_t *lcm = lcm_create(nullptr);

  // 事件订阅
  lcm_subscription_t *subscription_test0 = lcm_subscribe(lcm, "filter0", test_handler, 0);
  lcm_subscription_t *subscription_test1 = lcm_subscribe(lcm, "filter1", test_handler2, 0);
  lcm_subscription_t *subscription_all   = lcm_subscribe(lcm, ".*", catchall_handler, 0);

  // 获取文件描述子或套接字
  int fd = lcm_get_fileno(lcm);

  int num_transmitted = 0;
  while (num_transmitted < 1000) {
    /* Structure used in select() call, taken from the BSD file sys/time.h.
     * struct timeval
     * {
     *      long    tv_sec;         // seconds
     *      long    tv_usec;        // and microseconds
     * };
     */
    struct timeval timeout = {1, 0};

    /*
     * typedef struct fd_set
     * {
     *      u_int   fd_count;               // how many are SET?
     *      SOCKET  fd_array[FD_SETSIZE];   // an array of SOCKETs
     * } fd_set;
     * fd_array 的每一个元素都与一个已打开的 SOCKET 句柄建立联系，建立联系的工作由程序员完成
     * 当调用 select() 时，由内核根据 IO 状态修改 fd_set的内容，由此来同通知执行了 select()
     * 的进程哪一个 SOCKET 发生了可读或可写事件
     *
     * 常见用法
     *  fd_set set;
     *  FD_ZERO(&set);      // 将 set 清零，使得集合中不含任何 fd
     *  FD_SET(fd, *set);   // 将 fd 加入到 set 中
     *  FD_CLR(fd, *set);   // 将 fd 从集合中移除
     *  FD_ISSET(fd, &set); // 在调用select()函数后，用 FD_ISSET来检测 fd 是否在 set 集合中
     *                      // 当检测到 fd 在 set 中则返回真，否则返回假(0)
     *  select()是个轮询函数，循环询问文件节点，可设置超时事件，超时事件到了就跳过代码继续往下执行
     *          该函数可以做到很好的监听作用，监听读、写、报错
     *  int select(int nfds, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout)
     *      nfds        需要检查的文件描述子的个数
     *      readset     用来检查可读性的一组文件描述子
     *      writeset    用来检查可写性的文件描述子
     *      exceptset   用来检查是否有异常条件出现的文件描述子
     *      timeout     超时，值为NULL时为阻塞
     *
     *      return      返回 fd 的总数，错误时返回 SOCKET_ERROR
     */
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    int status = select(fd + 1, &readfds, 0, 0, &timeout);
    if (0 != status && FD_ISSET(fd, &readfds)) {
      std::cout << "status = 0B" << std::bitset<sizeof(int) * 8>(status) << std::endl;
      lcm_handle(lcm);
    }
  }

  lcm_unsubscribe(lcm, subscription_test0);
  lcm_unsubscribe(lcm, subscription_test1);
  lcm_unsubscribe(lcm, subscription_all);
  lcm_destroy(lcm);

  printf("transmitted: %d\n", num_transmitted);
  printf("test: %d test2: %d catchall: %d\n", test_count, test_count2, catchall_count);

  return 0;
}
