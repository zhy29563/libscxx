#include <lcm/lcm.h>
#include <stdio.h>

#include <thread>

#include "ex_example_t.h"

int main(int argc, char **argv) {
  lcm_t          *lcm          = lcm_create(NULL);
  lcm_eventlog_t *lcm_eventlog = NULL;

  lcm_eventlog = lcm_eventlog_create("../lcm/bin/lcmlog-2022-04-20.00", "r");
  if (lcm_eventlog == NULL) {
    return EXIT_FAILURE;
  }

  lcm_eventlog_event_t *lcm_eventlog_event = NULL;
  do {
    lcm_eventlog_event = lcm_eventlog_read_next_event(lcm_eventlog);
    if (lcm_eventlog_event != NULL) {
      printf("=========================================================================\n");
      printf("event num  = %ld\n", lcm_eventlog_event->eventnum);
      printf("timestamp  = %ld\n", lcm_eventlog_event->timestamp);
      printf("channel    = %s\n", lcm_eventlog_event->channel);
      printf("channellen = %d\n", lcm_eventlog_event->channellen);
      printf("datalen    = %d\n", lcm_eventlog_event->datalen);

      // 解析数据
      int          status;
      ex_example_t p;
      memset(&p, 0, sizeof(ex_example_t));
      status = ex_example_t_decode(lcm_eventlog_event->data, 0, lcm_eventlog_event->datalen, &p);
      if (status < 0) {
        fprintf(stderr, "error %d decoding exlcm_example_t!!!\n", status);
        return 1;
      }
      printf("=============================================================\n");
      printf("  timestamp = %ld\n", p.timestamp);
      printf("  position    = (%f, %f, %f)\n", p.position[0], p.position[1], p.position[2]);
      printf("  orientation = (%f, %f, %f, %f)\n", p.orientation[0], p.orientation[1], p.orientation[2],
             p.orientation[3]);
      printf("  ranges:");
      for (int i = 0; i < p.num_ranges; i++) printf(" %d", p.ranges[i]);
      printf("\n");
      printf("  name        = '%s'\n", p.name);
      printf("  enabled     = %d\n", p.enabled);

      ex_example_t_publish(lcm, "trans_file", &p);  /// 转发数据
      ex_example_t_decode_cleanup(&p);              /// 释放解码数据
      lcm_eventlog_free_event(lcm_eventlog_event);  /// 释放事件数据
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  } while (lcm_eventlog_event != NULL);

  if (lcm_eventlog != NULL) lcm_eventlog_destroy(lcm_eventlog);

  lcm_destroy(lcm);
  return EXIT_SUCCESS;
}
