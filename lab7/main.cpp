#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

#include "stitcher.h"

using namespace std;
using namespace cv;

const int K = 2;
const int MATCH_THRESHOLD = 25;
const float SIFT_RATIO_THRESH = 0.8;
const float DET_THRESH = 0.1;

int main( void ){
    // Create sift matcher
    Matcher sift;
    vector< KeyPoint > sift_kp1, sift_kp2;
    Mat sift_des1, sift_des2;
    sift.detector = SiftFeatureDetector::create();
    sift.matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    sift.setVars(MATCH_THRESHOLD, SIFT_RATIO_THRESH, K, DET_THRESH);

    // Create stitcher
    Stitcher stitcher = Stitcher("resources/Images/kitchen", "bmp", 66, sift);

    // Extract images, project to cylinder, match features and find translation
    vector< Mat > images = stitcher.extractImagesOrdered();
    vector< Mat > projImages = stitcher.projectImagesCylinder(images);
    vector< Mat > homographyMatrices = stitcher.getImageTranslation(projImages);

    return 0;
}