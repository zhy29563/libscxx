// Default arguments

#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i = 1, int j = 2) { return i + j; }

int sub(int i, int j) { return i - j; }

PYBIND11_MODULE(single_sample03, m) {
  // optional module docstring
  m.doc() = "pybind11 example plugin";
  m.def("add", &add, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2);

  using namespace pybind11::literals;
  m.def("sub", &sub, "A function which subs two numbers", "i"_a = 1, "j"_a = 2);
}