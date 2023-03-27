// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "utils.h"

using namespace cv;

int main(int argc, char** argv){
    if (argc < 2){
        std::cout << "[WARNING] Image name missing as command line argument. Aborting.\n";
        return 0;
    }

    Mat robo_img = imread(argv[1]); // Read the image given as argument in command line
    namedWindow("Robocup");
    imshow("Robocup", robo_img);
    
    setMouseCallback("Robocup", on_mouse_2, (void*)&robo_img);
    waitKey(0);

}