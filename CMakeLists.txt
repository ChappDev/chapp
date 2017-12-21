cmake_minimum_required(VERSION 2.8.7)
set(CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(chapp)
add_subdirectory(redis)
add_subdirectory(common)
add_subdirectory(server)

set(SOURCE_FILES test_db.cpp)

add_executable(test_db ${SOURCE_FILES})

target_link_libraries(test_db redis chapp_common)

