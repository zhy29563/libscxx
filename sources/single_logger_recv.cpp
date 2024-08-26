#include <inttypes.h>
#include <lcm/lcm.h>
#include <stdio.h>

#include "ex_example_t.h"

static void my_handler(const lcm_recv_buf_t *buffer, const char *channel, const ex_example_t *msg, void *user) {
  (void)buffer;
  (void)user;

  int i;
  printf("Received message on channel \"%s\":\n", channel);
  printf("  timestamp   = %" PRId64 "\n", msg->timestamp);
  printf("  position    = (%f, %f, %f)\n", msg->position[0], msg->position[1], msg->position[2]);
  printf("  orientation = (%f, %f, %f, %f)\n", msg->orientation[0], msg->orientation[1], msg->orientation[2],
         msg->orientation[3]);
  printf("  ranges:");
  for (i = 0; i < msg->num_ranges; i++) printf(" %d", msg->ranges[i]);
  printf("\n");
  printf("  name        = '%s'\n", msg->name);
  printf("  enabled     = %d\n", msg->enabled);
}

int main() {
  lcm_t *lcm = lcm_create(nullptr);
  if (!lcm) return 1;

  ex_example_t_subscribe(lcm, "trans_file", &my_handler, NULL);
  for (int i = 0; i < 10000; i++) lcm_handle(lcm);

  lcm_destroy(lcm);
  return 0;
}