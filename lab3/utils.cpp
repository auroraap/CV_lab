// utils.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "utils.h"

using namespace cv;

#define SAVE_IMAGES false
#define T 55

void on_mouse_2(int event, int x, int y, int f, void* userdata){
    // Task 2 callback
    if (event != EVENT_LBUTTONDOWN){
        return;
    }

    Mat img = *(Mat*) userdata;
    Point pixel(x,y);

    unsigned char b = img.at<Vec3b>(pixel)[0];
    unsigned char g = img.at<Vec3b>(pixel)[1];
    unsigned char r = img.at<Vec3b>(pixel)[2];

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

    int b_mean = img_mean[0];
    int g_mean = img_mean[1];
    int r_mean = img_mean[2];

    Scalar low(b_mean-T, g_mean-T, r_mean-T), high(b_mean+T, g_mean+T, r_mean+T);
    inRange(img_out, low, high, mask);

    if (SAVE_IMAGES) {
        imwrite("results/mask.jpg", mask);
    }

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

    // Initialize mask and define threshold
    Mat mask = Mat(img_out.rows, img_out.cols, CV_8UC1);

    int b_mean = img_mean[0];
    int g_mean = img_mean[1];
    int r_mean = img_mean[2];

    // Create mask using the threshold
    Scalar low(b_mean-T, g_mean-T, r_mean-T), high(b_mean+T, g_mean+T, r_mean+T);
    inRange(img_out, low, high, mask);
    
    // Set white pixels of mask to new color in result image
    Mat result = img_out.clone();
    result.setTo(Scalar(92, 37, 201), mask);

    if (SAVE_IMAGES) {
        imwrite("results/segmented.jpg", result);
    }

    namedWindow("Result");
    imshow("Result", result);
    waitKey(0);
}