cmake_minimum_required(VERSION 3.10)
project(echelonF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ecf SRecv.cpp SSend.cpp CSend.cpp CRecv.cpp functions.cpp main.cpp)
