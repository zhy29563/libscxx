#include <iomanip>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <vector>

template <class T>
static void PrintVector(const std::vector<T>& vec) {
  std::cout << "=========================================================================================" << std::endl;
  std::cout << "class name: " << typeid(std::vector<int>).name() << std::endl;
  std::cout << "size      : " << vec.size() << std::endl;
  std::cout << "empty     : " << vec.empty() << std::endl;
  std::cout << "capacity  : " << vec.capacity() << std::endl;
  std::cout << "max_size  : " << vec.max_size() << std::endl;
  std::cout << "data      : " << vec.data() << std::endl;

  for (std::size_t index = 0; index < vec.size(); index++) {
    std::cout << "Index: " << std::setw(3) << std::setfill('0') << index << ", Value: " << vec[index] << std::endl;
  }
}

int main() {
  // 创建
  std::vector<int> vec_int;
  PrintVector(vec_int);

  // 修改尺寸
  vec_int.resize(10);
  PrintVector(vec_int);

  // 增
  vec_int.push_back(10);
  vec_int.emplace_back(20);
  vec_int.insert(vec_int.begin(), 30);
  PrintVector(vec_int);

  int a = 100;
  vec_int.emplace_back(a);
  a                           = 200;
  vec_int[vec_int.size() - 1] = 400;
  std::cout << "a = " << a << std::endl;
  PrintVector(vec_int);

  // 改

  // 查

  // 删

  return EXIT_SUCCESS;
}