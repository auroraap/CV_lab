// utils.cpp

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include "utils.h"


cv::Mat max_filter(cv::Mat img, int kernel_size){
    cv::Mat out_img;

    if (kernel_size % 2 == 0) {
        std::cout << "[ERROR] Kernel size cannot be even numbered. Exiting.\n";
        return out_img;
    }

    out_img = img.clone(); //cv::Mat(img.rows, img.cols, cv::CV_8UC1);//img;
    int start_pos = (int) kernel_size/2;
    unsigned char max_val;

    std::cout << "[INFO] Max filter starting at pixel position (" << std::to_string(start_pos) << ", " << std::to_string(start_pos) << ")\n";

    for (int row = start_pos ; row < img.rows - start_pos; row++){
        for (int col = start_pos ; col < img.cols - start_pos; col++){
            
            // Loop over kernel
            max_val = 0;
            for (int kernel_row = row - start_pos; kernel_row <= row + start_pos; kernel_row++){
                for (int kernel_col = col - start_pos; kernel_col <= col + start_pos; kernel_col++){

                    unsigned char pixel_val = img.at<unsigned char>(kernel_row,kernel_col);
                    if (pixel_val > max_val){
                        max_val = pixel_val;
                    }

                }
            }

            out_img.at<unsigned char>(row,col) = max_val;
            
        }
    }

    cv::namedWindow("Max filter"); // Create a named window
    cv::imshow("Max filter", out_img); // Show the image in the named window
    cv::waitKey(0); // Wait for key-push to exit

    return out_img;
    
}

cv::Mat min_filter(cv::Mat img, int kernel_size){
    cv::Mat out_img;

    if (kernel_size % 2 == 0) {
        std::cout << "[ERROR] Kernel size cannot be even numbered. Exiting.\n";
        return out_img;
    }

    out_img = img.clone();
    int start_pos = (int) kernel_size/2;
    unsigned char min_val;

    std::cout << "[INFO] Min filter starting at pixel position (" << std::to_string(start_pos) << ", " << std::to_string(start_pos) << ")\n";

    for (int row = start_pos; row < img.rows - start_pos; row++){
        for (int col = start_pos ; col < img.cols - start_pos; col++){
            
            // Loop over kernel
            min_val = 255;
            for (int kernel_row = row - start_pos; kernel_row <= row + start_pos; kernel_row++){
                for (int kernel_col = col - start_pos; kernel_col <= col + start_pos; kernel_col++){

                    unsigned char pixel_val = img.at<unsigned char>(kernel_row,kernel_col);
                    if (pixel_val < min_val){
                        min_val = pixel_val;
                    }

                }
            }

            out_img.at<unsigned char>(row,col) = min_val;

        }
    }

    cv::namedWindow("Min filter"); // Create a named window
    cv::imshow("Min filter", out_img); // Show the image in the named window
    cv::waitKey(0); // Wait for key-push to exit

    return out_img;
}

cv::Mat draw_hist(cv::MatND hist, int width, int height, int hbins){

    cv::Mat histImage = cv::Mat(width, height, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    int bin_width = width / hbins;

    int p1x, p1y;
    int p2x, p2y;
    for (int i = 0; i < hbins; i++){
        // Draw the lines in the histogram by defining two points and connecting the dots
        // From x-position i-1 to i, with height according to the histogram
        p1x = bin_width*(i - 1);
		p1y = height - hist.at<float>(i - 1);

		p2x = bin_width*(i);
		p2y = height - hist.at<float>(i);

		line(histImage, cv::Point(p1x,p1y), cv::Point(p2x,p2y),
			cv::Scalar(255, 255, 255), 2, 8, 0);
    }

    return histImage;

}