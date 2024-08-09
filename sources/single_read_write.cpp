#include <json/json.h>

#include <fstream>
#include <iostream>
#include <string>

struct Person {
  int         age;
  std::string name;
};

void WriteJson(const std::string& file_name) {
  /*********************************************************************************************************************
   * ROOT
   ********************************************************************************************************************/
  Json::Value root;

  /*********************************************************************************************************************
   * basic_single
   ********************************************************************************************************************/
  Json::Value basic_single;
  basic_single["int8"]      = 1;
  basic_single["int16"]     = 2;
  basic_single["int32"]     = 3;
  basic_single["int64"]     = 4;
  basic_single["uint8"]     = 11;
  basic_single["uint16"]    = 21;
  basic_single["uint32"]    = 31;
  basic_single["uint64"]    = 41;
  basic_single["float"]     = 51;
  basic_single["double"]    = 52;
  basic_single["bool"]      = true;
  basic_single["cstring"]   = "cstring";
  basic_single["cppstring"] = std::string("cppstring");

  root["basic_single"] = basic_single;
  /*********************************************************************************************************************
   * basic_array
   ********************************************************************************************************************/
  Json::Value basic_array;
  for (int index = 0; index < 5; index++) {
    basic_array["int8_arr"][index] = index;
  }
  basic_array["int8_arr"].resize(8);
  basic_array["int8_arr"].append(8);
  basic_array["int8_arr"].insert(6, 9);

  for (int index = 0; index < 5; index++) {
    basic_array["uint8_arr"][index] = index;
  }
  basic_array["uint8_arr"].resize(8);
  basic_array["uint8_arr"].append(8);
  basic_array["uint8_arr"].insert(6, 9);

  for (int index = 0; index < 5; index++) {
    basic_array["cstring_arr"][index] = std::to_string(index);
  }
  basic_array["cstring_arr"].resize(8);
  basic_array["cstring_arr"].append(std::to_string(8));
  basic_array["cstring_arr"].insert(6, std::to_string(9));

  root["basic_array"] = basic_array;
  /*********************************************************************************************************************
   * object_single
   ********************************************************************************************************************/
  Json::Value object_single;
  object_single["name"] = "name";
  object_single["age"]  = 18;

  root["object_single"] = object_single;
  /*********************************************************************************************************************
   * object_array
   ********************************************************************************************************************/
  Json::Value object_array;
  for (int index = 0; index < 5; index++) {
    object_array[index]["name"] = "name_" + std::to_string(index);
    object_array[index]["age"]  = index;
  }

  root["object_array"] = object_array;

  /*********************************************************************************************************************
   * output
   ********************************************************************************************************************/
  // 直接输出
  std::cout << "FastWriter:" << std::endl;
  Json::FastWriter fw;
  std::cout << fw.write(root) << std::endl << std::endl;

  // 缩进输出
  std::cout << "StyledWriter:" << std::endl;
  Json::StyledWriter sw;
  std::cout << sw.write(root) << std::endl << std::endl;

  // 输出到字符串
  std::string jsonstr = sw.write(root);
  std::cout << jsonstr << std::endl;

  // 输出到文件
  std::ofstream os(file_name);
  if (!os.is_open()) {
    std::cout << "ERROR: can not create the json file: " << file_name << std::endl;
  }
  os << sw.write(root);
  os.flush();
  os.close();
}

void ReadJson(const std::string& file_name) {
  Json::Value root;

  std::ifstream ifs(file_name);
  if (!ifs.good() || !ifs.is_open()) {
    std::cout << "open json file is failed: " << file_name << std::endl;
    return;
  }
  Json::Reader reader;
  reader.parse(ifs, root);
  ifs.close();

  auto names = root.getMemberNames();
  for (const auto& name : names) {
    std::cout << "name: " << name << std::endl;
  }
  /*********************************************************************************************************************
   * basic_single
   ********************************************************************************************************************/
  Json::Value basic_single = root["basic_single"];
  if (basic_single["int8"].isInt()) {
    std::cout << "int8: " << basic_single["int8"].asInt() << std::endl;
  }

  if (basic_single["uint8"].isUInt()) {
    std::cout << "uint8: " << basic_single["uint8"].asUInt() << std::endl;
  }

  if (basic_single["int64"].isInt64()) {
    std::cout << "int64: " << basic_single["int64"].asInt64() << std::endl;
  }

  if (basic_single["uint64"].isUInt64()) {
    std::cout << "uint64: " << basic_single["uint64"].asUInt64() << std::endl;
  }

  if (basic_single["float"].isDouble()) {
    std::cout << "float: " << basic_single["float"].asFloat() << std::endl;
  }

  if (basic_single["double"].isDouble()) {
    std::cout << "double: " << basic_single["double"].asDouble() << std::endl;
  }

  if (basic_single["bool"].isBool()) {
    std::cout << "bool: " << basic_single["bool"].asBool() << std::endl;
  }
}

class JsonConfiguration {
 private:
  /// @brief 构造函数
  /// @param [in] json_ @ref Json::Value
  JsonConfiguration(Json::Value json_, const std::string& file_name_) : json(json_), file_name(file_name_) {}
  Json::Value json;
  std::string file_name;

 public:
  ~JsonConfiguration()                                          = default;
  JsonConfiguration(const JsonConfiguration& other)             = delete;
  JsonConfiguration(const JsonConfiguration&& other)            = delete;
  JsonConfiguration& operator=(const JsonConfiguration& other)  = delete;
  JsonConfiguration& operator=(const JsonConfiguration&& other) = delete;

  static int32_t LoadConfigFile(const std::string& file_name, std::shared_ptr<JsonConfiguration>& sp_configuration) {
    std::cout << "======================================================================================" << std::endl;
    std::cout << "JsonConfiguration, Load config file to json, file name: " << file_name << std::endl;

    std::ifstream ifs(file_name);
    if (!ifs.good()) {
      std::cout << "JsonConfiguration, Open config file is failed, file name: " << file_name << std::endl;
      return -1;
    }

    Json::Value json;
    ifs >> json;
    ifs.close();

    sp_configuration = std::shared_ptr<JsonConfiguration>(new JsonConfiguration(json, file_name));
    return 0;
  }

  template <class T>
  T GetValue(const std::string& key) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<T, std::string>, "ERROR");

    if (!this->json.isMember(key)) {
      std::cout << "The key[" << key << "] is not existed in file: " << this->file_name;
      return T();
    }

    auto json_v = this->json[key];
    if (json_v.isNull()) {
      std::cout << "The value of key[" << key << "] is null in file: " << this->file_name;
      return T();
    }

    return json_v.as<T>();
  }

  template <class T>
  std::vector<T> GetValueArray(const std::string& key) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<T, std::string>, "ERROR");

    if (!this->json.isMember(key)) {
      std::cout << "The key[" << key << "] is not existed." << this->file_name;
      return std::vector<T>();
    }

    auto json_v = this->json[key];
    if (json_v.isNull()) {
      std::cout << "The value of key[" << key << "] is null." << this->file_name;
      return std::vector<T>();
    }

    std::vector<T> results;
    if (json_v.is<T>()) {
      results.emplace_back(json_v.as<T>());
      return results;
    }

    if (!json_v.isArray()) {
      std::cout << "The value of key[" << key << "] is not array." << this->file_name;
      return std::vector<T>();
    }

    for (Json::ArrayIndex index = 0; index < json_v.size(); index++) {
      results.emplace_back(json_v[index].as<T>());
    }

    return results;
  }

  std::shared_ptr<JsonConfiguration> GetSubConfig(std::string key) {
    auto value_js = this->json[key.c_str()];
    if (value_js.isNull()) {
      return nullptr;
    }
    return std::shared_ptr<JsonConfiguration>(new JsonConfiguration(value_js, this->file_name));
  }

  std::shared_ptr<JsonConfiguration> GetSubConfig(int32_t key) {
    auto value_js = this->json[key];
    if (value_js.isNull()) {
      return nullptr;
    }
    return std::shared_ptr<JsonConfiguration>(new JsonConfiguration(value_js, this->file_name));
  }

  uint32_t ItemCount(void) { return this->json.size(); }

  std::vector<std::string> GetKeyNames() { return this->json.getMemberNames(); }
};

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  std::string json_file_name("test.json");
  // WriteJson(json_file_name);
  // ReadJson(json_file_name);

  std::shared_ptr<JsonConfiguration> sp_configuration = nullptr;
  JsonConfiguration::LoadConfigFile(json_file_name, sp_configuration);
  auto sp_basic_single = sp_configuration->GetSubConfig("basic_single");
  std::cout << "int8: " << sp_basic_single->GetValue<int>("int8") << std::endl;
  std::cout << "uint8: " << sp_basic_single->GetValue<unsigned int>("uint8") << std::endl;
  std::cout << "float: " << sp_basic_single->GetValue<float>("float") << std::endl;
  std::cout << "double: " << sp_basic_single->GetValue<double>("double") << std::endl;
  std::cout << "string: " << sp_basic_single->GetValue<std::string>("cppstring") << std::endl;

  auto sp_basic_array = sp_configuration->GetSubConfig("basic_array");
  auto int8_arr       = sp_basic_array->GetValueArray<int32_t>("int8_arr");
  auto uint8_arr      = sp_basic_array->GetValueArray<uint32_t>("uint8_arr");
  auto string_arr     = sp_basic_array->GetValueArray<std::string>("cstring_arr");

  auto sp_object_single = sp_configuration->GetSubConfig("object_single");
  std::cout << "name: " << sp_object_single->GetValue<std::string>("name") << std::endl;
  std::cout << "age : " << sp_object_single->GetValue<int32_t>("age") << std::endl;

  auto sp_object_array = sp_configuration->GetSubConfig("object_array");
  for (auto index = 0U; index < sp_object_array->ItemCount(); index++) {
    auto sp_object = sp_object_array->GetSubConfig(index);
    std::cout << "name: " << sp_object->GetValue<std::string>("name") << std::endl;
    std::cout << "age : " << sp_object->GetValue<int32_t>("age") << std::endl;
  }
  return EXIT_SUCCESS;
}
