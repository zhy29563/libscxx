# ######################################################################################################################
# 帮助
# ######################################################################################################################
# logcat
# dmesg

# ######################################################################################################################
# 项目设置
# ######################################################################################################################
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
get_filename_component(PROJECT_FOLDER_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component(PROJECT_FOLDER_NAME ${PROJECT_FOLDER_PATH} NAME)
project(${PROJECT_FOLDER_NAME} VERSION 0.1.0.0 LANGUAGES C CXX)

# ######################################################################################################################
# 导入
# ######################################################################################################################
include(GNUInstallDirs)
include(FindPkgConfig)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(${CMAKE_SOURCE_DIR}/resources/cmake/debug_marco.cmake)
include(${CMAKE_SOURCE_DIR}/resources/cmake/compile_parameters.cmake)
include(${CMAKE_SOURCE_DIR}/resources/cmake/configure_dependencies.cmake)

# ######################################################################################################################
# 编译相关
# ######################################################################################################################
option(ENABLE_CMAKE_DEBUG "是否启用CMAKE调试" ON)

# ######################################################################################################################
# 编译器选项
# ######################################################################################################################
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wdeprecated-declarations)

# ######################################################################################################################
# RPATH
# ######################################################################################################################
set(CMAKE_SKIP_RPATH OFF)
set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_SKIP_INSTALL_RPATH OFF)

# ######################################################################################################################
# 设置依赖库变量
# ######################################################################################################################
set(ENVS_ROOT ${CMAKE_SOURCE_DIR}/envs)
set(LIBS_NAME ${CMAKE_LIBRARY_ARCHITECTURE}-v${CMAKE_C_COMPILER_VERSION})
set(LIBS_ROOT ${ENVS_ROOT}/libs/${LIBS_NAME})
message(STATUS "LIBS_ROOT=${LIBS_ROOT}")

# ######################################################################################################################
# 依赖库
# ######################################################################################################################
set(glog_DIR   ${LIBS_ROOT}/libglog-v0.7.1/lib/cmake/glog)
find_package(glog 0.7.1 REQUIRED)
link_libraries(glog::glog)

# ######################################################################################################################
# 项目
# ######################################################################################################################
file(GLOB SUB_FILES LIST_DIRECTORIES FALSE ${CMAKE_CURRENT_SOURCE_DIR}/sources/*)

foreach(SUB_FILE ${SUB_FILES})
  get_filename_component(SUB_FILE_NAME ${SUB_FILE} NAME)

  if(SUB_FILE_NAME MATCHES "^single_.*\.cpp$")
    get_filename_component(FILE_NAME ${SUB_FILE} NAME_WE)
    add_executable(${FILE_NAME} ${SUB_FILE})
    target_include_directories(${FILE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  endif()
endforeach()
