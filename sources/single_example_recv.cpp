#include <lcm/lcm.h>

#include <iostream>

#include "Logger.h"
#include "ex_example_t.h"

static void my_handler([[maybe_unused]] const lcm_recv_buf_t *buf, const char *channel, const ex_example_t *msg,
                       [[maybe_unused]] void *user) {
  LOG_DEB("Received message on channel \"%s\":", channel);
  LOG_DEB("  timestamp   = %", msg->timestamp);
  LOG_DEB("  position    = (%f, %f, %f)", msg->position[0], msg->position[1], msg->position[2]);
  LOG_DEB("  orientation = (%f, %f, %f, %f)", msg->orientation[0], msg->orientation[1], msg->orientation[2],
          msg->orientation[3]);
  LOG_DEB("  ranges:");
  for (int i = 0; i < msg->num_ranges; i++) {
    LOG_DEB(" %d", msg->ranges[i]);
  }
  LOG_DEB("");
  LOG_DEB("  name        = %s", msg->name);
  LOG_DEB("  enabled     = %d", msg->enabled);
}

int main() {
  lcm_t *lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1");
  if (lcm == nullptr) {
    LOG_DEB("lcm_create failed");
    return EXIT_FAILURE;
  }

  ex_example_t_subscribe(lcm, "EXAMPLE", &my_handler, nullptr);

  while (true) {
    if (0 != lcm_handle(lcm)) {
      LOG_DEB("handle failed");
      break;
    }
  }

  lcm_destroy(lcm);
  return EXIT_SUCCESS;
}
