cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(common)

set(COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes)

set(SOURCE_FILES
  User.cpp
  Session.cpp
  Group.cpp
  Common.cpp

  includes/User.hpp
  includes/Session.hpp
  includes/Group.hpp
  includes/Common.hpp
  includes/Factories.hpp
  includes/Errors.hpp
  )

add_library(chapp_common ${SOURCE_FILES})

target_include_directories(chapp_common PUBLIC
        ${COMMON_INCLUDES}
        )

