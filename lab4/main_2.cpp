// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

// Canny
int T = 0;
const int T_max = 100;
const int ratio = 3;
const int kernel_size = 3;

// Hough
const int rho_h = 1;
const double theta = CV_PI / 180;
const int thresh = 200;

int main ( int argc,char** argv) {

	if (argc < 2 )
		return 0;

	Mat edges, cdst;
	std::vector<Vec2f> lines;
	
	Mat input_img_gray = imread(argv[1],IMREAD_GRAYSCALE);

	if(input_img_gray.empty())
	std::cout << "Image not found.\n";
		return 0;

	Canny(input_img_gray, edges, 50, 150);
	HoughLines(edges, lines, rho_h, theta, thresh);

	for( size_t i = 0; i < lines.size(); i++ )
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

	imshow("Source", input_img_gray);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);

	waitKey(0);
	return 0;
}
