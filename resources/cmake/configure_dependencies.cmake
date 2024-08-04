macro(add_libs_search_path)
  file(GLOB FIRST_FILES LIST_DIRECTORIES TRUE ${LIBS_ROOT}/*)

  foreach(FIRST_FILE ${FIRST_FILES})
    if(NOT IS_DIRECTORY ${FIRST_FILE})
      continue()
    endif()

    # 基于CMAKE的库查找路径
    if(${CMAKE_CROSSCOMPILING})
      list(APPEND CMAKE_FIND_ROOT_PATH ${FIRST_FILE})
    else()
      list(APPEND CMAKE_PREFIX_PATH ${FIRST_FILE})
    endif()

    # 基于PKGCONFIG的库查找路径
    set(EXCLUDE_FILES include bin etc)
    file(GLOB SECOND_FILES LIST_DIRECTORIES TRUE ${FIRST_FILE}/*)

    foreach(SECOND_FILE ${SECOND_FILES})
      if(NOT IS_DIRECTORY ${SECOND_FILE})
        continue()
      endif()

      get_filename_component(SECOND_FILE_NAME ${SECOND_FILE} NAME)
      list(FIND EXCLUDE_FILES ${SECOND_FILE_NAME} _FOUND_EXCLUDE)

      if(${_FOUND_EXCLUDE} GREATER_EQUAL 0)
        continue()
      endif()

      file(GLOB_RECURSE THIRD_FILES LIST_DIRECTORIES TRUE ${SECOND_FILE}/*)

      foreach(THIRD_FILE ${THIRD_FILES})
        get_filename_component(THIRD_FILE_NAME ${THIRD_FILE} NAME)

        if("${THIRD_FILE_NAME}" STREQUAL "pkgconfig")
          set(ENV{PKG_CONFIG_PATH} "${THIRD_FILE}:$ENV{PKG_CONFIG_PATH}")
        endif()
      endforeach()
    endforeach()
  endforeach()

  print_environment_variable(PKG_CONFIG_PATH)

  if(${CMAKE_CROSSCOMPILING})
    print_list_variable(CMAKE_FIND_ROOT_PATH)
  else()
    print_list_variable(CMAKE_PREFIX_PATH)
  endif()
endmacro()

macro(find_lib_by_pkgconfig FILE_NAME_PC)
  pkg_search_module(LIB_${FILE_NAME_PC} REQUIRED ${FILE_NAME_PC})

  if(${LIB_${FILE_NAME_PC}_FOUND})
    if(${ENABLE_CMAKE_DEBUG})
      print_all_variables_with_regex(LIB_${FILE_NAME_PC})
    endif()

    include_directories(${LIB_${FILE_NAME_PC}_INCLUDE_DIRS})
    link_directories(${LIB_${FILE_NAME_PC}_LIBRARY_DIRS})
    link_libraries(${LIB_${FILE_NAME_PC}_LIBRARIES})
  else()
    message(FATAL_ERROR "Can not found ${FILE_NAME_PC}")
  endif()
endmacro()

macro(find_lib_by_cmake LIB_NAME)
  if(${ARGC} GREATER 1)
    foreach(COMPONENT ${ARGN})
      find_package(${LIB_NAME} REQUIRED COMPONENTS ${COMPONENT})

      if(${${LIB_NAME}_FOUND})
        if(${ENABLE_CMAKE_DEBUG})
          print_all_variables_with_regex(${LIB_NAME})
        endif()

        include_directories(${${LIB_NAME}_INCLUDE_DIRS})
        link_directories(${${LIB_NAME}_LIBRARY_DIRS})
        link_libraries(${${LIB_NAME}_LIBRARIES})
      else()
        message(FATAL_ERROR "Can not found ${LIB_NAME}:${COMPONENT}")
      endif()
    endforeach()
  else()
    find_package(${LIB_NAME} REQUIRED)

    if(${${LIB_NAME}_FOUND})
      if(${ENABLE_CMAKE_DEBUG})
        print_all_variables_with_regex(${LIB_NAME})
      endif()

      include_directories(${${LIB_NAME}_INCLUDE_DIRS})
      link_directories(${${LIB_NAME}_LIBRARY_DIRS})
      link_libraries(${${LIB_NAME}_LIBRARIES})
    else()
      message(FATAL_ERROR "Can not found ${LIB_NAME}")
    endif()
  endif()
endmacro()

macro(find_qt_by_cmake LIB_NAME)
  foreach(COMPONENT ${ARGN})
    find_package(${LIB_NAME} REQUIRED COMPONENTS ${COMPONENT})

    if(${${LIB_NAME}${COMPONENT}_FOUND})
      if(${ENABLE_CMAKE_DEBUG})
        print_all_variables_with_regex("${LIB_NAME}${COMPONENT}")
      endif()

      include_directories(${${LIB_NAME}${COMPONENT}_INCLUDE_DIRS})
      link_directories(${${LIB_NAME}${COMPONENT}_LIBRARY_DIRS})
      link_libraries(${${LIB_NAME}${COMPONENT}_LIBRARIES})
    else()
      message(FATAL_ERROR "Can not found ${LIB_NAME}:${COMPONENT}")
    endif()
  endforeach()
endmacro()

macro(find_thread_by_cmake)
  find_package(Threads REQUIRED)

  if(${Threads_FOUND})
    if(${ENABLE_CMAKE_DEBUG})
      print_all_variables_with_regex(THREAD)
    endif()

    link_libraries(Threads::Threads)
  else()
    message(FATAL_ERROR "Can not found Threads")
  endif()
endmacro()
