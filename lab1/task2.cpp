// helloworld.cpp

#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "[WARNING] No image name provided by user. Aborting.\n";
        return 0;
    }

    Mat img = imread(argv[1]); // Read the image given as argument in command line

    // Get the number of channels. This is one of the attributes of cv::Mat.
    int ch = img.channels();
    std::cout << "[INFO] Number of image channels: " << ch << "\n";

    if (img.data == NULL){
        std::cout << "[WARNING] Cannot find the provided filename. Aborting.\n";
        return 0;
    }

    namedWindow("Example 1"); // Create a named window
    imshow("Example 1", img); // Show the image in the named window
    char waitKeyOutput = waitKey(0); // Wait for key-push to exit

    std::cout << "[INFO] Key pressed: " << waitKeyOutput << "\n";

    return 0;
}