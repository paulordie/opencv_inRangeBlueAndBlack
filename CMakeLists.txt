cmake_minimum_required(VERSION 3.17)
project(inRangeBlueAndBlack)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_FOUND 1)
find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.cpp)

add_executable(inRangeBlueAndBlack ${SOURCE_FILES})

target_link_libraries(inRangeBlueAndBlack ${OpenCV_LIBS})