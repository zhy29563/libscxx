# ######################################################################################################################
# 编译参数
# ######################################################################################################################
# 指定C语言标准
# 90(C89/C90) 99(C99) 11(C11) 17(C17)[V3.21] 23(C23)[V3.21]
set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED TRUE )
set ( CMAKE_C_EXTENSIONS TRUE )

# 指定C++语言标准
# 98(C++98) 11(C++11) 14(C++14) 17(C++17)[V3.8] 20(C++20)[V3.12] 23(C++23)[V3.20] 26(C++26)[V3.25]
set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set ( CMAKE_CXX_EXTENSIONS TRUE )

# 包含${CMAKE_SOURCE_DIR}与${CMAKE_BINARY_DIR}到头文件搜索路径
set ( CMAKE_INCLUDE_CURRENT_DIR TRUE )

# 生成位置独立代码
set ( CMAKE_POSITION_INDEPENDENT_CODE TRUE )

# 导出compile_command.json文件
set ( CMAKE_EXPORT_COMPILE_COMMANDS TRUE )

# 生成详细makefile
set ( CMAKE_VERBOSE_MAKEFILE TRUE )

# 设置默认编译类型为RelWithDebInfo，即 -o2 -g
if ( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
  set ( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE )
  set_property ( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" )
endif ()

# 设置输出目录
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
