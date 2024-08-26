#include <glib.h>
#include <lcm/lcm.h>

#include "Logger.h"
#include "ex_example_t.h"

int main() {
  lcm_t* lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1");
  if (lcm == nullptr) {
    LOG_DEB("lcm_create failed");
    return EXIT_FAILURE;
  }

  ex_example_t my_data = {
      .timestamp   = g_get_real_time(),
      .position    = {1, 2, 3},
      .orientation = {1, 0, 0, 0},
  };

  int16_t ranges[15];
  for (int16_t i = 0; i < 15; i++) {
    ranges[i] = i;
  }

  my_data.num_ranges = 15;
  my_data.ranges     = ranges;
  my_data.name       = "example string";
  my_data.enabled    = 1;

  ex_example_t_publish(lcm, "EXAMPLE", &my_data);
  LOG_DEB("Published EXAMPLE");

  lcm_destroy(lcm);
  return EXIT_SUCCESS;
}
