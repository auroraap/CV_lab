// utils.cpp

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "utils.h"

using namespace cv;

void on_mouse(int event, int x, int y, int f, void* userdata){
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    
    // ---------------- Task 2 ----------------
    int b = img.at<Vec3b>(x,y)[0];
    int g = img.at<Vec3b>(x,y)[1];
    int r = img.at<Vec3b>(x,y)[2];

    std::cout << "[Task2] BGR: " << std::to_string(b) << ", " << std::to_string(g) << ", " << std::to_string(r) << "\n";

}