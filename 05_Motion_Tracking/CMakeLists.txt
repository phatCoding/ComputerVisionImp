# This Project was build with Generator MinGw and CXX-Compiler g++

cmake_minimum_required(VERSION 3.2)

project( Motion_Tracker )

set(MY_SOURCE_DIR ./src/)
set(MY_INCLUDE_DIR ./include/)

find_package(${INCLUDE_DIR} OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${MY_INCLUDE_DIR})

add_library(MyLib05 
        "${MY_SOURCE_DIR}/bodyframe.cpp"
        "${MY_SOURCE_DIR}/trace.cpp")

add_executable(Motion_Tracker "${MY_SOURCE_DIR}/main.cpp" ) 

target_link_libraries(Motion_Tracker PRIVATE MyLib05 ${OpenCV_LIBS})
