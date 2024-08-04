/***********************************************************************************************************************
 * 1. 过滤日志
 *    VLOG(m)与标志v配合使用，仅输出m小于等于v标志指定值的日志
 *    例如：如果设置了v标志为1，那么VLOG(1), VLOG(2)中仅有VLOG(1)输出
 *    VLOG 使用INFO日志等级
 *
 * 2. vmodule
 *    指定特定模块（文件）的等级，例如：-vmodule=file_one=2,file_two=1,gfs*=3 --v=0
 *      输出file_one.{h,cc}文件中VLOG模式中等级小于等于2的日志
 *      输出file_two.{h,cc}文件中VLOG模式中等级小于等于1的日志
 *      输出所有名称以gfs开头的文件中VLOG模式中等级小于等于3的日志
 *        * 匹配零个或多个字符
 *        ? 匹配单个字符
 *      输出其余模块VLOG模式中等级小于等于0的日志
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

  VLOG(1) << "I’m printed when you run the program with --v=1 or higher";
  VLOG(2) << "I’m printed when you run the program with --v=2 or higher";

  google::ShutdownGoogleLogging();
  return 0;
}
