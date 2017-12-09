cmake_minimum_required(VERSION 3.8)
project(client_common)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH /Users/alex/Documents/frameworks/Qt/5.9.2/clang_64/) #Находим все либы Qt

set(CMAKE_INCLUDE_CURRENT_DIR ON) #Включаем поиск в текущей директории
set(CMAKE_AUTOMOC ON) #Включаем автосоздание моков, когда они нужны

#Ищем нужные в проекте либы
find_package(Qt5Core)
find_package(Qt5Network)


set(SOURCE_FILES main.cpp Client.cpp Client.h Connection.cpp Connection.h RequestCmd.cpp RequestCmd.h DiffiHellmanCmd.cpp DiffiHellmanCmd.h Command.cpp Command.h)
add_executable(client_common ${SOURCE_FILES})

#Присоединяем либы к проекту к проекту
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Network)