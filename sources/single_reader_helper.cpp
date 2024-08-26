#include <dmlc/json.h>

#include <fstream>
#include <iostream>
#include <sstream>

struct Param {
  std::string name;
  int         value{};
  inline void Load(dmlc::JSONReader *reader) {
    dmlc::JSONObjectReadHelper helper;
    helper.DeclareField("name", &name);
    helper.DeclareField("value", &value);
    helper.ReadAllFields(reader);
  }
};

int main() {
  std::string       json_string = R"({"name":"apple","value":100})";
  std::stringstream ss;
  ss << json_string;
  dmlc::JSONReader jr(&ss);

  Param p;
  p.Load(&jr);
  std::cout << "name : " << p.name << std::endl;
  std::cout << "value: " << p.value << std::endl;

  return 0;
}
