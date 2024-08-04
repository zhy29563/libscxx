/***********************************************************************************************************************
 * 1. 使用标志控制日志行为
 *    启用 logtostderr，则可以通过以下方式进行设置：
 *    命令行： GLOG_logtostderr=1 ./your_application
 *    程序内： FLAGS_logtostderr=1
 *
 * 2. 常用标志
 *    logtostderr     (bool,   default=false)                 是否将日志输出到标准错误流
 *    stderrthreshold (int,    default=2, which is ERROR)     输出到标准错误流的日志等级
 *    minloglevel     (int,    default=0, which is INFO)      输出日志的最小等级
 *    log_dir         (string, default="")                    日志存储目录（如果指定的目录不存在，则自动创建）
 *    v               (int,    default=0)                     VLOG模式中输出小于等于指定数字等级的日志
 *    vmodule         (string, default="")                    <module name>=<log level>指定模块的日志等级
 *
 * 3. 注意
 *    minloglevel： 如果程序内未显示设置该标志，则可使用使用命令行参数动态修改； 否则，修改无效
 *    log_dir    ： 如果程序内未显示设置该标志，则可使用使用命令行参数动态修改； 否则，修改无效
 *               ： 必须在初始化之前设置
 *               ： 指定的目录必须事先创建，否则报错
 *    v          ： 如果程序内未显示设置该标志，则可使用使用命令行参数动态修改； 否则，修改无效
 *               ： VLOG(N1) GLOG_v=N2，如果N1 <= N2 输出
 **********************************************************************************************************************/
#include <glog/logging.h>

int main(int argc, char* argv[]) {
  (void)argc;

  FLAGS_log_dir          = "logs";
  FLAGS_minloglevel      = 0;
  FLAGS_alsologtostderr  = true;
  FLAGS_colorlogtostdout = true;
  FLAGS_colorlogtostderr = true;
  // FLAGS_v                = -2;

  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "info";
  LOG(WARNING) << "warning";
  LOG(ERROR) << "error";
  VLOG(-1) << "VLOG(-1)";
  VLOG(-2) << "VLOG(-2)";
  VLOG(-3) << "VLOG(-3)";

  google::ShutdownGoogleLogging();
  return 0;
}
