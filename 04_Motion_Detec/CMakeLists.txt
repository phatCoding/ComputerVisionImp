# This Project was build with Generator MinGw and CXX-Compiler g++

cmake_minimum_required(VERSION 3.2)

project( Motion_Detec )

set(MY_OBJDETEC_DIR src/objectdetection/)
set(MY_SOURCE_DIR src/)
set(MY_UTIL_DIR src/util/)
set(MY_INCLUDE_DIR include/)

find_package(${INCLUDE_DIR} OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${MY_INCLUDE_DIR})

add_library(MyLib 
        "${MY_UTIL_DIR}/PreProcImage.cpp"
        "${MY_UTIL_DIR}/Paint.cpp"
        "${MY_UTIL_DIR}/Calculation.cpp"
        "${MY_OBJDETEC_DIR}/trackedobjectmonitor.cpp"
        "${MY_OBJDETEC_DIR}/trackedobject.cpp"
)

add_executable(Motion_Detec "${MY_SOURCE_DIR}/main.cpp") 

target_link_libraries(Motion_Detec PRIVATE MyLib  ${OpenCV_LIBS})