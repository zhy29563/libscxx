#include <lcm/lcm.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

int main() {
  lcm_t *lcm = lcm_create(nullptr);

  int data_sz = sizeof(int);
  for (int i = 0; i < 10000; i++) {
    char *data = (char *)calloc(1, data_sz);

    // 格式化循环变量成字符串
    sprintf(data, "%d", i);

    const char *channel = i % 2 == 0 ? "filter0" : "filter1";
    lcm_publish(lcm, channel, data, data_sz + 1);
    printf("transmitted msg # %5d size %d\n", i, data_sz);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    free(data);
  }

  return 0;
}