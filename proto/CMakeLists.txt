cmake_minimum_required(VERSION 2.8.7)

project(proto)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER chapp.proto)

add_library(proto
        ${PROTO_HEADER}
        ${PROTO_SRC})

target_link_libraries(proto PUBLIC ${PROTOBUF_LIBRARIES})

target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})