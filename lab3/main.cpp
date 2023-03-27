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

    int task;
    std::cout << "Input task to view (1/2/3/4/5): ";
    std::cin >> task; // Get user input from the keyboard

    Mat robo_img = imread(argv[1]); // Read the image given as argument in command line
    namedWindow("Robocup");
    imshow("Robocup", robo_img);

    if (task == 2){
        setMouseCallback("Robocup", on_mouse_2, (void*)&robo_img);
    }
    else if (task == 3){
        setMouseCallback("Robocup", on_mouse_3, (void*)&robo_img);
    }
    else if (task == 4){
        setMouseCallback("Robocup", on_mouse_4, (void*)&robo_img);
    }
    else if (task == 5){
        setMouseCallback("Robocup", on_mouse_5, (void*)&robo_img);
    }
    
    waitKey(0);

}