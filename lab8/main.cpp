#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

#include "cam_calibration.h"

using namespace cv;
using namespace std;

const int sidelen = 10;
const int innerRow = 6;
const int innerCol = 5;

int main( int argc, char** argv ){
    // Load images from folder
    vector<cv::String> filenames;
    glob("data/checkerboard_images/*.png", filenames, false); // get filenames

    // Allow user to specify number of images for calibration
    int n_images = filenames.size();
    cout << "Total number of images: " << to_string(n_images) << "\n";
    if ( argc > 1 ) {
        int n_input = atoi( argv[1] );
        if (n_input < n_images) {
            n_images = n_input;
        }
    }
    cout << "Number of images selected by user as command line argument: " << to_string(n_images) << "\n";

    vector< Mat > images, result;

    CamCalibrator calibrator = CamCalibrator(innerCol, innerRow, sidelen);
    calibrator.setup();
    calibrator.getCorners(filenames, &images, n_images, false);
    calibrator.setCalibrationParams( images[0].size() );
    int reprojectionError = calibrator.computeReprojectionError();
    cout << "Reprojection error: " << to_string(reprojectionError) << "\n";
    calibrator.undistortAndRectify(images, &result, images[0].size(), true);

}