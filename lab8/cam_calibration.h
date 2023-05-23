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