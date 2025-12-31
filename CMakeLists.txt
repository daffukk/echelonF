cmake_minimum_required(VERSION 3.10)
project(echelonF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EServer SRecv.cpp SSend.cpp EServer.cpp)
add_executable(EClient CSend.cpp CRecv.cpp EClient.cpp)
