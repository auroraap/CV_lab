// utils.cpp

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat max_filter(cv::Mat img, int kernel_size);

cv::Mat min_filter(cv::Mat img, int kernel_size);

cv::Mat draw_hist(cv::MatND hist, int width, int height, int hbins);