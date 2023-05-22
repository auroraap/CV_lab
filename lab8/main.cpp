#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv ){
    int innerRow = 6;
    int innerCol = 5;
    // ~~~~~~~ PART 1 ~~~~~~~ -> Load
    cout << "Part 1\n";
    // Load images from folder
    vector<cv::String> filenames;
    vector< Mat > distImages, undistImages;
    glob("data/checkerboard_images/*.png", filenames, false); // get filenames

    // Allow user to specify number of images for calibration
    size_t n_images = filenames.size();
    if ( argc >= 2 ) {
        size_t n_input = (size_t) argv[1];
        if (n_input < n_images) {
            n_images = n_input;
        }
    }

    // ~~~~~~~ PART 2 ~~~~~~~ -> Find chessboard corners
    cout << "Part 2\n";
    vector < vector < Point2f > > imagePoints;
    vector < Point2f > cornerPoints;
    vector < vector < Point3f > > objectPoints;
    vector < Point3f > obj;
    for(int i{0}; i<innerRow; i++){
        for(int j{0}; j<innerCol; j++) {
            obj.push_back(cv::Point3f(j,i,0));
        } 
    }

    Size patternSize = Size(innerRow,innerCol);
    Mat img_gray;
    for (size_t i=0; i<n_images; i++) {
        Mat img = imread(filenames[i]);
        distImages.push_back(img);
        cvtColor(img, img_gray, COLOR_BGR2GRAY);

        bool success = findChessboardCorners(img_gray, patternSize, cornerPoints);

        if ( success ) {
            // Possibly refine with cornerSubPix
            objectPoints.push_back(obj);
            imagePoints.push_back(cornerPoints);
        }
    }

    // ~~~~~~~ PART 3 ~~~~~~~ -> Calibrate camera
    cout << "Part 3\n";
    Mat cameraMatrix, distCoeffs, R, T;
    calibrateCamera(objectPoints, imagePoints, img_gray.size(), cameraMatrix, distCoeffs, R, T);

    // ~~~~~~~ PART 4 ~~~~~~~ -> Compute reprojection error
    cout << "Part 4\n";
    // int mean_error = 0;
    // vector < Point2f > projImagePoints;
    // for (int i = 0; i < objectPoints.size(); i++){
    //     projectPoints(objectPoints[i], R, T, cameraMatrix, distCoeffs, projImagePoints);
    //     int error = norm(imagePoints[i], projImagePoints, NORM_L2) / projImagePoints.size();
    //     mean_error = mean_error + error;
    // }
    // mean_error = mean_error / objectPoints.size();

    // ~~~~~~~ PART 5 ~~~~~~~ -> Undistort and rectify
    cout << "Part 5\n";
    Mat map1, map2, undist, newCam;
    //newCam = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, img_gray.size(), 1, img_gray.size(), 0);
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), img_gray.size(), CV_32FC1, map1, map2);
    for ( int i = 0; i < distImages.size(); i++ ){
        remap( distImages[i], undist, map1, map2, INTER_CUBIC );
        undistImages.push_back(undist);
    }

    // ~~~~~~~ PART 6 ~~~~~~~ -> Show distorted and undistorted image in one window
    Mat dst;
    hconcat(distImages[0], undistImages[0], dst);
    imshow("Images", dst);
    waitKey(0);

}