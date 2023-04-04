// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

// Canny
const int T_low = 50;
const int T_high = 150;
const int kernel_size = 3;

// Hough
const double rho_hough = 1;
const double theta_hough = CV_PI / 180;
const int T = 200;

int main ( int argc,char** argv) {

	if (argc < 2 )
		return 0;

	Mat edges, cdst, input_image_gray_blurred;
	std::vector<Vec2f> lines;
	
	Mat input_img_gray = imread(argv[1],IMREAD_GRAYSCALE);

	if(input_img_gray.empty()){
		std::cout << "Image not found.\n";
		return 0;
	}
	
	GaussianBlur(input_img_gray, input_image_gray_blurred, Size(11,11), 0); // Noise reduction
	Canny(input_image_gray_blurred, edges, T_low, T_high, kernel_size); // Detect edges
	HoughLines(edges, lines, rho_hough, theta_hough, T); // Detect lines among the detected edges
	cvtColor(input_img_gray, cdst, COLOR_GRAY2BGR); // Make cdst a 3-channel copy of the input image

	for( size_t i = 0; i < lines.size(); i++ ) // Draw lines in cdst
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }

	namedWindow("Source");
	namedWindow("Blurred");
	namedWindow("Detected Edges");
	namedWindow("Detected Lines");
	imshow("Source", input_img_gray);
	imshow("Blurred", input_image_gray_blurred);
	imshow("Detected Edges", edges);
    imshow("Detected Lines", cdst);

	waitKey(0);
	return 0;
}
