// main.cpp

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "utils.h"

int main(int argc, char** argv)
{
    if (argc < 3){
        std::cout << "[WARNING] Image name or kernel size missing as command line arguments. Please run as './Lab2 image.jpg kernel_size'. Aborting.\n";
        return 0;
    }
    int kernel_size;

    cv::Mat original_img = cv::imread(argv[1]); // Read the image given as argument in command line
    kernel_size = std::atoi(argv[2]);

    std::cout << "[INFO] Running with kernel size " << std::to_string(kernel_size) << "\n";

    if (original_img.empty()){
        std::cout << "[WARNING] Cannot find the provided filename. Aborting.\n";
        return 0;
    }

    // ---------- TASK 1 ----------
    // Show original image
    cv::namedWindow("Original image"); // Create a named window
    cv::imshow("Original image", original_img); // Show the image in the named window
    cv::waitKey(0); // Wait for key-push to exit

    // Convert to grayscale
    cv::Mat grayscale_img;
    cv::cvtColor(original_img, grayscale_img, cv::COLOR_BGR2GRAY, 1);
    cv::imwrite("results/image_grayscale.jpg", grayscale_img);
    
    // ---------- TASK 2 ----------
    // Run the implemented min and max filters: Kernel size 5 is ok
    cv::Mat filtered_image;
    cv::Mat min_image, max_image;

    max_image = max_filter(grayscale_img, kernel_size);
    cv::imwrite("results/image_max.jpg", max_image);
    min_image = min_filter(grayscale_img, kernel_size);
    cv::imwrite("results/image_min.jpg", min_image);

    // Continue using the max filtered image
    filtered_image = max_image;
    
    // ---------- TASK 3 ----------
    // Use median filter and Gaussian smoothing filter
    cv::Mat median, gauss;
    cv::medianBlur(filtered_image, median, kernel_size);
    cv::imwrite("results/image_median.jpg", median);

    cv::GaussianBlur(filtered_image, gauss, cv::Size(kernel_size, kernel_size), 0.5);

    cv::imwrite("results/image_gauss.jpg", gauss);

    cv::namedWindow("Gauss filtered image"); // Create a named window
    cv::imshow("Gauss filtered image", gauss); // Show the image in the named window
    cv::namedWindow("Median filtered image"); // Create a named window
    cv::imshow("Median filtered image", median); // Show the image in the named window
    cv::waitKey(0); // Wait for key-push to exit

    // ---------- TASK 4 ----------
    // Histogram of grayscale image
    cv::MatND hist;
    const int hbins = 256;
    float hrange[] = { 0, 255 };
    const float *histRange = { hrange };

    cv::calcHist(&grayscale_img, 1, 0, cv::Mat(), hist, 1, &hbins, &histRange, true, false);
    
    int width = 256, height = 300;
    cv::Mat histImage = draw_hist(hist, width, height, hbins);

    cv::imwrite("results/image_histogram.jpg", histImage);
    cv::namedWindow("Histogram");
    cv::imshow("Histogram", histImage);
    cv::waitKey(0); // Wait for key-push to exit

    // ---------- TASK 5 ----------
    cv::Mat equalized_img;
    cv::equalizeHist( grayscale_img, equalized_img );

    cv::imwrite("results/image_equalized.jpg", equalized_img);
    cv::namedWindow("Equalized image");
    cv::imshow("Equalized image", equalized_img);
    cv::waitKey(0); // Wait for key-push to exit

    // Draw histogram of equalized image
    cv::MatND eqHist;
    cv::calcHist(&equalized_img, 1, 0, cv::Mat(), eqHist, 1, &hbins, &histRange, true, false);
    cv::Mat eqHistImage = draw_hist(eqHist, width, height, hbins);

    cv::imwrite("results/image_equalized_histogram.jpg", eqHistImage);
    cv::namedWindow("Equalized histogram");
    cv::imshow("Equalized histogram", eqHistImage);
    cv::waitKey(0); // Wait for key-push to exit

    return 0;
}