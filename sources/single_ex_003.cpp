/***********************************************************************************************************************
 * 1. 条件日志
 *    LOG_IF
 *
 * 2. 周期日志
 *    google::COUNTER 记录循环次数。
 *      从1开始
 *      仅在周期日志中有效，其他值为零
 *    LOG_FIRST_N
 *    LOG_EVERY_N
 *    LOG_EVERY_T
 *
 * 3. 条件周期日志
 *    LOG_IF_EVERY_N
 **********************************************************************************************************************/
#include <glog/logging.h>

#include <iostream>
#include <thread>

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
    LOG_IF(INFO, i > 5) << "LOG_IF(INFO, i > 5):" << i << ", COUNT: " << google::COUNTER;
  }

  // 输出前N条日志
  for (size_t i = 0; i < 10; i++) {
    LOG_FIRST_N(INFO, 5) << "LOG_FIRST_N(INFO, 5): " << google::COUNTER;
  }

  // 周期日志 使用 google::COUNTER 记录输出日志的循环量
  for (size_t i = 0; i < 10; i++) {
    LOG_EVERY_N(INFO, 3) << "LOG_EVERY_N(INFO, 3): " << i << ", COUNT: " << google::COUNTER;
  }

  // 时间周期输出（3ms）
  for (size_t i = 0; i < 10; i++) {
    LOG_EVERY_T(INFO, 0.003) << "LOG_EVERY_T(INFO, 0.003): " << i;
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }

  // 组合条件日志与周期日志
  for (size_t i = 0; i < 10; i++) {
    LOG_IF_EVERY_N(INFO, (i > 5), 3) << "LOG_IF_EVERY_N(INFO, (i > 5), 3): " << google::COUNTER;
  }
  google::ShutdownGoogleLogging();
  return 0;
}
