/***********************************************************************************************************************
 * 1. 调试日志
 *    DLOG_IF
 *
 * 2. 周期日志
 *    google::COUNTER 记录循环次数。
 *      从1开始
 *      仅在周期日志中有效，其他值为零
 *    DLOG_FIRST_N
 *    DLOG_EVERY_N
 *    DLOG_EVERY_T
 *
 * 3. 条件周期日志
 *    DLOG_IF_EVERY_N
 **********************************************************************************************************************/
#include <glog/logging.h>

int main(int argc, char* argv[]) {
  (void)argc;

  FLAGS_log_dir          = "logs";
  FLAGS_minloglevel      = 0;
  FLAGS_alsologtostderr  = true;
  FLAGS_colorlogtostdout = true;
  FLAGS_colorlogtostderr = true;

  google::InitGoogleLogging(argv[0]);

  // 条件日志
  for (size_t i = 0; i < 10; i++) {
    DLOG_IF(INFO, i > 5) << "DLOG_IF(INFO, i > 5):" << i;
  }

  // 周期日志 使用 google::COUNTER 记录输出日志的循环量
  for (size_t i = 0; i < 10; i++) {
    // DLOG(INFO) << google::COUNTER; // 输出：0
    DLOG_EVERY_N(INFO, 3) << "DLOG_EVERY_N(INFO, 3): " << google::COUNTER;
  }

  // 组合条件日志与周期日志
  for (size_t i = 0; i < 10; i++) {
    DLOG_IF_EVERY_N(INFO, (i > 5), 3) << "DLOG_IF_EVERY_N(INFO, (i > 5), 3): " << google::COUNTER;
  }

  google::ShutdownGoogleLogging();
  return 0;
}
