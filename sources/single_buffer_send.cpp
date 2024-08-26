#include <lcm/lcm.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

int main() {
  lcm_t *lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1");

  int data_sz = sizeof(int);
  for (int i = 0; i < 10000; i++) {
    char *data = (char *)calloc(1, data_sz);
#ifdef WIN32
    sprintf_s(data, data_sz, "%d", i);
#elif linux
    snprintf(data, data_sz, "%d", i);
#elif __sun
    snprintf(data, data_sz, "%d", i);
#endif

    lcm_publish(lcm, "BUFFER_TEST", data, data_sz + 1);
    printf("transmitted msg # %5d size %d\n", i, data_sz);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    free(data);
  }

  return 0;
}