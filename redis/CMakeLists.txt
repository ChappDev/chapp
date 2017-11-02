cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(redis)

add_subdirectory(cpp_redis)

set(DATABASE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes)

find_path(CPPREDIS_INCLUDE_DIR cpp_redis/cpp_redis PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cpp_redis/includes/)

find_path(TACOPIE_INCLUDE_DIR tacopie/tacopie PATHS ${PROJECT_SOURCE_DIR}/tacopie/includes/)

set(SOURCE_FILES Database.cpp includes/Database.h)
add_library(redis ${SOURCE_FILES})

target_include_directories(redis PUBLIC
        ${DATABASE_INCLUDES}
        ${CPPREDIS_INCLUDE_DIR}
        ${TACOPIE_INCLUDE_DIR}
        )
target_link_libraries(redis PUBLIC cpp_redis)
