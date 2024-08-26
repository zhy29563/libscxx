#include "Logger.h"
#include "person_repeated.pb.h"

int main() {
  // 声明与初始化
  PersonRepeated person;
  person.set_name("zhang san");
  person.set_age(20);
  person.add_phone_number("1234567890");
  person.add_phone_number("1234567891");

  // 对象序列化
  std::string serialized_string;
  person.SerializeToString(&serialized_string);

  // 显示序列化结果
  LOG_DEB("serialized_string=%s", serialized_string.c_str());
  LOG_DEB("=======================================================");

  // 反序列化
  PersonRepeated response;
  response.ParseFromString(serialized_string);

  // 显示反序列化结果
  LOG_DEB("name = %s", response.name().c_str());
  LOG_DEB("age  = %d", response.age());

  int size = response.phone_number_size();
  for (int i = 0; i < size; ++i) {
    LOG_DEB("phone_number = %s", response.phone_number(i).c_str());
  }

  return EXIT_SUCCESS;
}
