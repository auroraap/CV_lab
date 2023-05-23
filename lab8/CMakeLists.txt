cmake_minimum_required(VERSION 2.8)
project( Lab8 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Lab8
    ./main.cpp
    ./cam_calibration.cpp
)
target_link_libraries( Lab8 ${OpenCV_LIBS} )