// 自定义日志前缀
/***********************************************************************************************************************
 * 1. 自定义日志前缀
 **********************************************************************************************************************/
#include <glog/logging.h>
#include <time.h>

#include <iomanip>
#include <iostream>

void CustomPrefix(std::ostream &s, const google::LogMessage &m, void *userdata) {
  (void)userdata;
  s << google::GetLogSeverityName(m.severity())[0]         // 日志等级
    << std::setw(4) << 1900 + m.time().year()              // 年
    << std::setw(2) << 1 + m.time().month()                // 月
    << std::setw(2) << m.time().day() << ' '               // 日
    << std::setw(2) << m.time().hour() << ':'              // 时
    << std::setw(2) << m.time().min() << ':'               // 分
    << std::setw(2) << m.time().sec() << "."               // 秒
    << std::setw(6) << m.time().usec() << ' '              // 微秒
    << std::setfill(' ') << std::setw(5) << m.thread_id()  // 线程号
    << std::setfill('0') << ' ' << m.basename() << ':'     // 文件名
    << m.line()                                            // 行号
    << "]";
}

int main(int argc, char *argv[]) {
  (void)argc;

  FLAGS_log_dir          = "logs";
  FLAGS_minloglevel      = 0;
  FLAGS_alsologtostderr  = true;
  FLAGS_colorlogtostdout = true;
  FLAGS_colorlogtostderr = true;

  google::InstallPrefixFormatter(&CustomPrefix);
  google::InitGoogleLogging(argv[0]);

  for (size_t i = 0; i < 10; i++) {
    LOG_IF(INFO, i > 5) << "LOG_IF(INFO, i > 5):" << i;
  }

  google::ShutdownGoogleLogging();
  return 0;
}
