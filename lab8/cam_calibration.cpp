#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class CamCalibrator {
    private:
        int innerCols;
        int innerRows;
        int squareSize;
        vector < Mat > R, T;
        Mat cameraMatrix, distCoeffs;
        vector < Point3f > obj;
        vector < vector < Point2f > > imagePoints;
        vector < vector < Point3f > > objectPoints;
    public:
        CamCalibrator (int c, int r, int s){
            innerCols = c;
            innerRows = r;
            squareSize = s;
        };
        void setup( );
        void getCorners( vector<String> filenames, vector< Mat >* images, int n_images, bool showImages );
        void setCalibrationParams( Size imageSize );
        int computeReprojectionError(  );
        void undistortAndRectify( vector< Mat > distortedImageVec, vector< Mat >* undistortedImageVec, Size imagesSize, bool showImage  );
};

void CamCalibrator::setup(  ) {
    for(int i = 0; i < innerRows; i++){ // Create objectpoints
        for(int j = 0; j < innerCols; j++) {
            Point3f pt = Point3f( j*squareSize,i*squareSize,0 );
            obj.push_back( pt );
        } 
    }
}

void CamCalibrator::getCorners( vector<String> filenames, vector< Mat >* images, int n_images, bool showImages ){
    Mat img_gray;
    vector < Point2f > cornerPoints;
    for (size_t i=0; i < n_images; i++) {
        Mat img = imread(filenames[i]);
        cvtColor(img, img_gray, COLOR_BGR2GRAY);

        bool success = findChessboardCorners(img_gray, Size(innerRows, innerCols), cornerPoints);

        if ( success ) {
            cornerSubPix(img_gray,cornerPoints, Size(11,11), Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));
            objectPoints.push_back(obj);
            imagePoints.push_back(cornerPoints);
            images->push_back(img);

            if (showImages) {
                Mat chess = img.clone();
                drawChessboardCorners(chess, Size(innerRows, innerCols), cornerPoints, success);
                imshow("Result", chess);
                waitKey(0);
            }
        }
    }
}

void CamCalibrator::setCalibrationParams( Size imageSize ){
    calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, R, T);
    cout << "Camera Matrix: " << cameraMatrix << "\n";
    cout << "Distortion Coefficients: " << distCoeffs << "\n";
}

int CamCalibrator::computeReprojectionError(){
    int sum_error = 0;
    vector < Point2f > projImagePoints;
    for (int i = 0; i < objectPoints.size(); i++){
        projectPoints(objectPoints[i], R[i], T[i], cameraMatrix, distCoeffs, projImagePoints);
        int error = norm(imagePoints[i], projImagePoints, NORM_L2) / projImagePoints.size();
        sum_error = sum_error + error;
    }
    return sum_error / objectPoints.size();
}

void CamCalibrator::undistortAndRectify( vector< Mat > distortedImageVec, vector< Mat >* undistortedImageVec, Size imagesSize, bool showImage ){
    Mat mapx, mapy;
    Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imagesSize, 1, imagesSize, 0);
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), newCameraMatrix, imagesSize, CV_16SC2, mapx, mapy);

    for ( size_t i = 0; i < distortedImageVec.size(); i++ ){
        Mat undist, result;
        remap( distortedImageVec[i], undist, mapx, mapy, INTER_LINEAR );
        hconcat(distortedImageVec[i], undist, result); // Create side-by-side image
        undistortedImageVec->push_back(result);
        if (showImage){
            imshow("Distorted", distortedImageVec[i]);
            imshow("Undistorted", undist);
            waitKey(0);
        }
    }
}