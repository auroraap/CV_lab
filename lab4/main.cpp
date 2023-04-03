// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int T = 10;
const int ratio = 3;
const int kernel_size = 3;
const int t_max = 255;

void on_trackbar(int, void* userdata)
{
    cv::Mat img = *(cv::Mat*) userdata;
    cv::Mat canny_img;
    cv::Canny( img, canny_img, T, T*ratio, kernel_size );
    cv::imshow( "Canny", img );
}

int main(int argc, char** argv){
    if (argc < 2){
        std::cout << "[WARNING] Image name missing as command line argument. Aborting.\n";
        return 0;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if(img.empty())
	    std::cout << "Image could not be read.\n";
        return 0;

    cv::namedWindow("Canny"); // Create Window
    cv::createTrackbar("Threshold","Canny", &T, t_max, on_trackbar);
    on_trackbar(0, (void*)&img);

    cv::waitKey(0);

    return 0;

}