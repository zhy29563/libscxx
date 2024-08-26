#include "Logger.h"
#include "person_simple.pb.h"

int main() {
  // 声明与初始化
  PersonSimple p;
  p.set_name("zhang san");
  p.set_age(20);

  // 对象序列化
  std::string serialized_string;
  p.SerializeToString(&serialized_string);

  // 显示序列化结果
  LOG_DEB("serialized_string=%s", serialized_string.c_str());
  LOG_DEB("=======================================================");

  // 反序列化
  PersonSimple response;
  response.ParseFromString(serialized_string);

  // 显示反序列化结果
  LOG_DEB("name = %s", response.name().c_str());
  LOG_DEB("age  = %d", response.age());

  return EXIT_SUCCESS;
}
