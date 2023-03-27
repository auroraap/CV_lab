// utils.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "utils.h"

using namespace cv;

void on_mouse_2(int event, int x, int y, int f, void* userdata){
    // Task 2 callback
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    
    int b = img.at<Vec3b>(x,y)[0];
    int g = img.at<Vec3b>(x,y)[1];
    int r = img.at<Vec3b>(x,y)[2];

    std::cout << "[Task2] BGR: " << std::to_string(b) << ", " << std::to_string(g) << ", " << std::to_string(r) << "\n";
}

void on_mouse_3(int event, int x, int y, int f, void* userdata){
    // Task 3 callback
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    Mat img_out = img.clone();
    
    int neighborhood_size = 9;
    if (y + neighborhood_size > img_out.rows || x + neighborhood_size > img_out.cols){
        return;
    }

    Rect neighborhood = Rect(x, y, neighborhood_size, neighborhood_size);
    Scalar img_mean = mean(img_out(neighborhood));

    std::cout << "[Task3] BGR Means: " << img_mean << "\n";
}

void on_mouse_4(int event, int x, int y, int f, void* userdata){
    // Task 4 callback
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    Mat img_out = img.clone();
    
    int neighborhood_size = 9;
    if (y + neighborhood_size > img_out.rows || x + neighborhood_size > img_out.cols){
        return;
    }

    Rect neighborhood = Rect(x, y, neighborhood_size, neighborhood_size);
    Scalar img_mean = mean(img_out(neighborhood));
    Mat mask = Mat(img_out.rows, img_out.cols, CV_8UC1);
    int T = 55;

    int b_mean = img_mean[0];
    int g_mean = img_mean[1];
    int r_mean = img_mean[2];

    inRange(img_out, Scalar(b_mean-T, g_mean-T, r_mean-T), Scalar(b_mean+T, g_mean+T, r_mean+T), mask);
    
    namedWindow("Mask");
    imshow("Mask", mask);
    waitKey(0);
}

void on_mouse_5(int event, int x, int y, int f, void* userdata){
    // Task 5 callback
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    Mat img_out = img.clone();
    
    int neighborhood_size = 9;
    if (y + neighborhood_size > img_out.rows || x + neighborhood_size > img_out.cols){
        return;
    }

    Rect neighborhood = Rect(x, y, neighborhood_size, neighborhood_size);
    Scalar img_mean = mean(img_out(neighborhood));
    Mat mask = Mat(img_out.rows, img_out.cols, CV_8UC1);
    int T = 60;

    int b_mean = img_mean[0];
    int g_mean = img_mean[1];
    int r_mean = img_mean[2];

    inRange(img_out, Scalar(b_mean-T, g_mean-T, r_mean-T), Scalar(b_mean+T, g_mean+T, r_mean+T), mask);

    Mat result = img_out.clone();
    result.setTo(Scalar(92, 37, 201), mask);

    namedWindow("Result");
    imshow("Result", result);
    waitKey(0);
}