// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int T = 0;
const int T_max = 100;
const int ratio = 3;
const int kernel_size = 3;
const std::string window = "Canny"; //Window Name

cv::Mat input_img,output_img; //input and output images;
cv::Mat canny_img; //Edge Detected image;

static void on_trackbar(int,void*)
{
	cv::GaussianBlur(input_img, canny_img, cv::Size(kernel_size,kernel_size), 0); // Blurring image to reduce noise
	cv::Canny(canny_img, canny_img, T, T * ratio, kernel_size);
	output_img = cv::Scalar::all(0);
	input_img.copyTo(output_img, canny_img);
	cv::imshow(window, output_img);
}


int main ( int argc,char** argv) {

	if (argc < 2 )
		return 0;

	input_img = cv::imread(argv[1],cv::IMREAD_GRAYSCALE);

	if(input_img.empty())
		return 0;

	output_img.create(input_img.size(),input_img.type());
	cv::namedWindow(window);
	cv::createTrackbar("Threshold: ",window,&T,T_max,on_trackbar);
	on_trackbar(0,0);

	cv::waitKey(0);
	return 0;
}