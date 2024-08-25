// Creating bindings for a simple function

#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) { return i + j; }

/***********************************************************************************************************************
 * PYBIND11_MODULE 宏创建一个在使用 import 语句进行导入时被调用的函数
 * 宏的第一个参数指定模块的名称，一定不能使用引号
 * 宏的第二个参数定义了一个类型为'py::module_'类型的变量m，这个用于创建语言绑定的主要接口
 * 方法'module_::def()' 生成绑定代码
 *
 * 验证：
 * cd build
 * python
 *  import os
 *  import sys
 *  sys.path.append(os.path.realpath('.'))
 *  import single_sample01
 **********************************************************************************************************************/
PYBIND11_MODULE(single_sample01, m) {
  // optional module docstring
  m.doc() = "pybind11 example plugin";
  m.def("add", &add, "A function which adds two numbers");
}
