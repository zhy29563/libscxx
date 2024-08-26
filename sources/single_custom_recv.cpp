#include <lcm/lcm.h>

#include <cinttypes>

#include "ex_example_t.h"

static void my_handler(const lcm_recv_buf_t *buffer, const char *channel, const ex_example_t *msg, void *user) {
  (void)buffer;
  (void)user;

  printf("============================================================\n");
  printf("Received message on channel \"%s\":\n", channel);
  printf("  timestamp   = %" PRId64 "\n", msg->timestamp);
  printf("  position    = (%f, %f, %f)\n", msg->position[0], msg->position[1], msg->position[2]);
  printf("  orientation = (%f, %f, %f, %f)\n", msg->orientation[0], msg->orientation[1], msg->orientation[2],
         msg->orientation[3]);
  printf("  ranges:");
  for (int i = 0; i < msg->num_ranges; i++) printf(" %d", msg->ranges[i]);
  printf("\n");
  printf("  name        = '%s'\n", msg->name);
  printf("  enabled     = %d\n", msg->enabled);
}

int main() {
  lcm_t *lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1?ifa=192.168.1.177");
  if (!lcm) return 1;

  ex_example_t_subscribe(lcm, "custome", &my_handler, nullptr);
  for (int i = 0; i < 10000; i++) lcm_handle(lcm);

  lcm_destroy(lcm);
  return 0;
}