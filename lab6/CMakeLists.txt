cmake_minimum_required(VERSION 2.8)
project( Lab6 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Lab6
    ./main.cpp
    ./matcher.cpp
)
target_link_libraries( Lab6 ${OpenCV_LIBS} )