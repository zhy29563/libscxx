#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

template <class T>
static void PrintVector(const std::vector<T>& vec, const std::string& description) {
  std::cout << "=========================================================================================" << std::endl;
  std::cout << "description: " << description << std::endl;
  std::cout << "class name : " << typeid(std::vector<int>).name() << std::endl;
  std::cout << "size       : " << vec.size() << std::endl;
  std::cout << "empty      : " << vec.empty() << std::endl;
  std::cout << "capacity   : " << vec.capacity() << std::endl;
  std::cout << "max_size   : " << vec.max_size() << std::endl;
  std::cout << "data       : " << vec.data() << std::endl;

  for (std::size_t index = 0; index < vec.size(); index++) {
    std::cout << "Index: " << std::setw(3) << std::setfill('0') << index << ", Value: " << vec[index] << std::endl;
  }
}

int main() {
  {
    std::vector<int> vec_pod_one;
    PrintVector(vec_pod_one, "create vec_pod_one");

    vec_pod_one.resize(5);
    PrintVector(vec_pod_one, "resize vec_pod_one");

    std::vector<int> vec_pod_two(5);
    PrintVector(vec_pod_two, "create vec_pod_two");
  }

  {
    std::vector<int> vec_pod;
    vec_pod.emplace_back(20);
    vec_pod.emplace_back(30);
    vec_pod.emplace_back(40);
    PrintVector(vec_pod, "pod emplace_back");

    vec_pod.insert(vec_pod.begin(), 50);
    PrintVector(vec_pod, "pod insert");
  }

  {
    int              a = 10;
    std::vector<int> vec_pod;
    vec_pod.emplace_back(a);
    a                           = 200;
    vec_pod[vec_pod.size() - 1] = 400;
    std::cout << "a = " << a << std::endl;
    PrintVector(vec_pod, "modify pod");
  }

  {
    std::vector<int> vec_pod{1, 4, 6, 2, 7, 4};
    PrintVector(vec_pod, "before sort");

    std::sort(vec_pod.begin(), vec_pod.end());
    PrintVector(vec_pod, "after sort");
  }

  {
    std::vector<int> vec_pod{1, 4, 6, 2, 7, 4};
    PrintVector(vec_pod, "before find");

    auto iter = std::find(vec_pod.begin(), vec_pod.end(), 4);
    if (iter == vec_pod.end()) {
      std::cout << "find is failed." << std::endl;
    } else {
      std::ptrdiff_t distance = std::distance(vec_pod.begin(), iter);
      std::cout << "distance is: " << distance << std::endl;
    }
  }

  return EXIT_SUCCESS;
}