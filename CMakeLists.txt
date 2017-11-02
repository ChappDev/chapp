cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(chapp)

set(SOURCE_FILES test_db.cpp)


add_executable(test_db ${SOURCE_FILES})

add_subdirectory(redis)
target_link_libraries(test_db redis)