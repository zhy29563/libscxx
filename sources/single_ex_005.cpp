/***********************************************************************************************************************
 * 1. 运行时检测
 *    如果条件满足，不做任何处理
 *    如果条件不满足，输出日志后终止程序
 *    不受NDEBUG控制
 *
 * 2. 基础宏
 *    CHECK
 *
 * 3. 比较宏
 *    CHECK_EQ
 *    CHECK_NE
 *    CHECK_LE
 *    CHECK_LT
 *    CHECK_GE
 *    CHECK_GT
 *
 * 4. 指针比较
 *    CHECK_EQ(some_ptr, static_cast<SomeType*>(nullptr));
 *    CHECK_NOTNULL
 *
 * 5. C字符串比较
 *    HECK_STREQ
 *    CHECK_STRNE
 *    CHECK_STRCASEEQ
 *    CHECK_STRCASENE
 *
 * 6. 浮点数比较
 *    CHECK_DOUBLE_EQ
 **********************************************************************************************************************/
#include <glog/logging.h>

#include <iostream>

int main(int argc, char* argv[]) {
  (void)argc;

  FLAGS_log_dir          = "logs";
  FLAGS_minloglevel      = 0;
  FLAGS_alsologtostderr  = true;
  FLAGS_colorlogtostdout = true;
  FLAGS_colorlogtostderr = true;

  google::InitGoogleLogging(argv[0]);

  for (size_t i = 0; i < 10; i++) {
    CHECK(i < 5) << "CHECK(i < 5):" << i;
  }

  google::ShutdownGoogleLogging();
  return 0;
}
