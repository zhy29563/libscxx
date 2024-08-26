#include <lcm/lcm.h>

#include <thread>

#include "ex_example_t.h"

int main() {
  lcm_t       *lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1?ifa=192.168.1.177");
  ex_example_t my_data;

  my_data.position[0] = 1;
  my_data.position[1] = 2;
  my_data.position[2] = 3;

  my_data.orientation[0] = 1;
  my_data.orientation[1] = 0;
  my_data.orientation[2] = 0;
  my_data.orientation[3] = 0;

  int16_t ranges[15];
  for (int i = 0; i < 15; i++) ranges[i] = i;

  my_data.num_ranges = 15;
  my_data.ranges     = ranges;
  my_data.name       = (char *)"example string";
  my_data.enabled    = 1;

  for (size_t i = 0; i < 10000; i++) {
    my_data.timestamp = i;
    ex_example_t_publish(lcm, "custome", &my_data);
    printf("The timestamp of published data is %ld\n", my_data.timestamp);

    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }

  lcm_destroy(lcm);
  return 0;
}
