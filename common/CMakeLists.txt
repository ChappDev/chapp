cmake_minimum_required(VERSION 3.8)
project(common)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH /home/hetrock/alex/opt/Qt/5.9.2/gcc_64/) #Находим все либы Qt

set(CMAKE_INCLUDE_CURRENT_DIR ON) #Включаем поиск в текущей директории
set(CMAKE_AUTOMOC ON) #Включаем автосоздание моков, когда они нужны

#Ищем нужные в проекте либы
find_package(Qt5Core)
find_package(Qt5Network)


set(SOURCE_FILES groups/AbstractGroup.cpp groups/AbstractGroup.hpp groups/PublicGroup.cpp groups/PublicGroup.hpp groups/PrivateGroup.cpp groups/PrivateGroup.hpp groups/GroupCredentials.cpp groups/GroupCredentials.hpp Message.cpp Message.hpp groups/InviteGroup.cpp groups/InviteGroup.hpp fabrics/GroupFabric.cpp fabrics/GroupFabric.hpp User.hpp User.cpp Session.hpp Session.cpp main.cpp)
add_executable(common ${SOURCE_FILES})

#Присоединяем либы к проекту
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Network)