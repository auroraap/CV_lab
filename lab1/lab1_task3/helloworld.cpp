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
    
    // Fetch argument to determine which channel should be removed
    int removeChannel = 0;
    if (argc > 2){
        removeChannel = (int) *argv[2];
    }
    else {
        std::cout << "[INFO] No channel to remove selected. Defaulting to 0.\n";
    }

    // Get the number of channels. This is one of the attributes of cv::Mat.
    int ch = img.channels();
    std::cout << "[INFO] Number of image channels: " << ch << "\n";

    if (ch == 3){
        // Set first channel to 0
        for (int i = 0; i < img.rows; i++){
            for (int j = 0; j < img.cols; j++){
                // Figured: Encoding is bgr. Look at color wheel. 
                // The color that is the most clear in resulting image is the one that is cancelled by b/g/r.
                img.at<Vec3b>(i,j)[removeChannel] = 0;

            }
        }
    }

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