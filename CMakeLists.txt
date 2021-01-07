cmake_minimum_required(VERSION 3.17)
project(ArnoldLogSystem)

set(CMAKE_CXX_STANDARD 14)
include_directories("C:/Xilinx/Vivado/2019.2/include")

add_executable(ArnoldLogSystem main.cpp ConversionEngine.cpp A_LogNum.cpp A_LogNum.h)