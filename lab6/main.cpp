
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

#include "matcher.h"

using namespace cv;
using namespace std;

const int K = 2;
const int MATCH_THRESHOLD = 25;
const float ORB_RATIO_THRESH = 0.8;
const float SIFT_RATIO_THRESH = 0.8;
const float SURF_RATIO_THRESH = 0.8;
const float DET_THRESH = 0.1;
const int SURF_HESSIAN_THRESH = 400;

int main( int argc, char** argv ){
    if (argc < 2 ){
        cout << "Make sure to include two image names in the command line.\n";
        return 0;
    }
		
	
	Mat img1 = imread(argv[1], IMREAD_GRAYSCALE);
    Mat img2 = imread(argv[2], IMREAD_GRAYSCALE);

    if ((img1.empty()) or (img2.empty())){
        cout << "The provided images were not found.\n";
        return 0;
    }
    // ~~~~~~~~~ SIFT ~~~~~~~~~ 
    cout << "SIFT\n";
    Matcher sift;
    vector< KeyPoint> sift_kp1, sift_kp2;
    Mat sift_des1, sift_des2;
    sift.detector = SiftFeatureDetector::create();
    sift.matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    sift.setVars(MATCH_THRESHOLD, SIFT_RATIO_THRESH, K, DET_THRESH);
    sift.buildDescriptor(img1, img2, &sift_kp1, &sift_kp2, &sift_des1, &sift_des2);
    vector<DMatch> sift_matches = sift.matchKeypoints(sift_des1, sift_des2);
    sift.checkSimilarity(sift_matches, sift_kp1, sift_kp2);

    // ~~~~~~~~~ SURF ~~~~~~~~~
    cout << "SURF\n";
    Matcher surf;
    vector< KeyPoint> surf_kp1, surf_kp2;
    Mat surf_des1, surf_des2;
    surf.detector = xfeatures2d::SURF::create( SURF_HESSIAN_THRESH );
    surf.matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    surf.setVars(MATCH_THRESHOLD, SURF_RATIO_THRESH, K, DET_THRESH);
    surf.buildDescriptor(img1, img2, &surf_kp1, &surf_kp2, &surf_des1, &surf_des2);
    vector<DMatch> surf_matches = surf.matchKeypoints(surf_des1, surf_des2);
    surf.checkSimilarity(surf_matches, surf_kp1, surf_kp2);

    // ~~~~~~~~~ ORB ~~~~~~~~~
    cout << "ORB\n";
    Matcher orb;
    vector< KeyPoint> orb_kp1, orb_kp2;
    Mat orb_des1, orb_des2;
    orb.detector = ORB::create();
    orb.matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_L1);
    orb.setVars(MATCH_THRESHOLD, ORB_RATIO_THRESH, K, DET_THRESH);
    surf.buildDescriptor(img1, img2, &orb_kp1, &orb_kp2, &orb_des1, &orb_des2);
    vector<DMatch> orb_matches = orb.matchKeypoints(orb_des1, orb_des2);
    orb.checkSimilarity(orb_matches, orb_kp1, orb_kp2);

    Mat sift_result = sift.drawResult(img1, img2, sift_kp1, sift_kp2, sift_matches);
    Mat surf_result = surf.drawResult(img1, img2, surf_kp1, surf_kp2, surf_matches);
    Mat orb_result = orb.drawResult(img1, img2, orb_kp1, orb_kp2, orb_matches);
    imwrite("SIFT.jpg", sift_result);
    imshow("SIFT", sift_result);
    imwrite("SURF.jpg", surf_result);
    imshow("SURF", surf_result);
    imwrite("ORB.jpg", orb_result);
    imshow("ORB", orb_result);
    waitKey(0);
    
}