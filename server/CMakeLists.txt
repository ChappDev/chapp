cmake_minimum_required(VERSION 3.8)
project(server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON) #Включаем поиск в текущей директории
set(CMAKE_AUTOMOC ON) #Включаем автосоздание моков, когда они нужны

#Ищем нужные в проекте либы
find_package(Qt5Core)
find_package(Qt5Network)
find_library(GMP_LIBRARY gmp)
find_library(GMPXX_LIBRARY gmpxx)
find_package(OpenSSL)


set(SOURCE_FILES main.cpp AesEncoder.cpp AesEncoder.h Server.cpp Server.hpp Client.cpp Client.h RequestQueue.cpp RequestQueue.h Command.cpp Command.h DiffieHellmanWrapper.cpp DiffieHellmanWrapper.h DiffieHellmanInit.cpp DiffieHellmanInit.h DiffieHellmanCalcSharedKey.cpp DiffieHellmanCalcSharedKey.h)

add_executable(server ${SOURCE_FILES})

#Присоединяем либы к проекту к проекту
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
target_link_libraries(${PROJECT_NAME} "${GMP_LIBRARY}" "${GMPXX_LIBRARY}")
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
