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

    if (ch == 3){
        // Create 3 images - All with one channel. Keep value of one channel each.
        Mat image_1(img.rows,img.cols,CV_8UC1);
        Mat image_2(img.rows,img.cols,CV_8UC1);
        Mat image_3(img.rows,img.cols,CV_8UC1);

        for (int i = 0; i < img.rows; i++){
            for (int j = 0; j < img.cols; j++){
                image_1.at<unsigned char>(i,j) = img.at<Vec3b>(i,j)[0];
                image_2.at<unsigned char>(i,j) = img.at<Vec3b>(i,j)[1];
                image_3.at<unsigned char>(i,j) = img.at<Vec3b>(i,j)[2];
            }
        }
        namedWindow("Image 1"); // Create a named window
        namedWindow("Image 2");
        namedWindow("Image 3");

        imshow("Image 1", image_1); // Show the image in the named window
        imshow("Image 2", image_2);
        imshow("Image 3", image_3);
    }
    else{
        namedWindow("Image");
        imshow("Imega", img);
    }

    char waitKeyOutput = waitKey(0); // Wait for key-push to exit
    std::cout << "[INFO] Key pressed: " << waitKeyOutput << "\n";

    return 0;
}