cmake_minimum_required(VERSION 2.8.7)

set(CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(common)

set(COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes)

set(SOURCE_FILES
  User.cpp
  Session.cpp
  Group.cpp
  Common.cpp
  Util.cpp
  UserFactory.cpp

  includes/User.hpp
  includes/Session.hpp
  includes/Group.hpp
  includes/Common.hpp
  includes/UserFactory.hpp
  includes/Errors.hpp
  includes/Phash.hpp
  includes/Util.hpp
  )

if(${NO_DATABASE})
  set(SOURCE_FILES
    ${SOURCE_FILES}
    test-includes/GroupFactory.hpp
    )
endif(${NO_DATABASE})

add_library(chapp_common ${SOURCE_FILES})

if(${NO_DATABASE})
  target_include_directories(chapp_common PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test-includes
  )
  target_compile_definitions(chapp_common PRIVATE
    NO_DATABASE
  )
else()
  find_path(DATABASE_INCLUDES Database.hpp
    paths ${CMAKE_SOURCE_DIR}/redis/includes)

  target_include_directories(chapp_common PUBLIC
    ${DATABASE_INCLUDES}
    )

  target_link_libraries(chapp_common redis)
endif(${NO_DATABASE})

target_include_directories(chapp_common PUBLIC
        ${COMMON_INCLUDES}
        )

