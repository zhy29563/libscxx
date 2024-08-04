/***********************************************************************************************************************
 * 1. 日志等级：
 *    INFO(0)
 *    WARNING(1)
 *    ERROR(2)
 *    FATAL(3)
 *
 * 2. 高等级的日志不仅出现对应等级的日志文件中，亦出现在等级比其低的日志中
 * 3. DFATAL日志等级在DEBUG模式下为FATAL，在其他默认中为ERROR
 * 4. 默认情况下，日志文件的存在路径为：/tmp/program_name.hostname.user name.log.severity_level.date-time.pid
 * 5. 默认情况下，GLOG将等级为ERROR或FATAL的日志拷贝到标准错误流
 **********************************************************************************************************************/
#include <glog/logging.h>

int main(int argc, char* argv[]) {
  (void)argc;

  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "info";
  LOG(WARNING) << "warning";
  LOG(ERROR) << "error";
  /*LOG(FATAL) << "fatal";*/
  google::ShutdownGoogleLogging();
  return 0;
}
