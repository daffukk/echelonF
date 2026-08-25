cmake_minimum_required(VERSION 3.10)
project(ecf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
  src/client/send.cpp
  src/client/receive.cpp
  src/server/send.cpp
  src/server/receive.cpp
  src/main.cpp 
  src/network.cpp
  src/terminal.cpp
  src/utils.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
