#include <lcm/lcm.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include "Logger.h"

int num_received = 0;
int msg_id       = 0;

static void on_buffer_test(const lcm_recv_buf_t *buffer, const char *channel, void *user_data) {
  (void)channel;
  (void)(user_data);

  num_received++;
  msg_id = atoi(static_cast<char *>(buffer->data));
  LOG_DEB("received msg id: %4d (recv count: %4d)", msg_id, num_received);
  if (num_received % 10 == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    LOG_DEB("sleeping for 1000 ms");
  }
}

int main() {
  lcm_t              *lcm          = lcm_create("udpm://239.255.76.67:7667?ttl=1");
  lcm_subscription_t *subscription = lcm_subscribe(lcm, (char *)"BUFFER_TEST", on_buffer_test, nullptr);

  int queue_capacity = 10;
  int ret            = lcm_subscription_set_queue_capacity(subscription, queue_capacity);
  printf("The return of lcm_subscription_set_queue_capacity: %d\n", ret);

  while (lcm_handle(lcm) == 0) {
    if (msg_id >= 9000) {
      break;
    }
  }

  return 0;
}