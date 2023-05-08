// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const int K = 2;
const int MATCH_THRESHOLD = 10;
const float ORB_RATIO_THRESH = 0.85;
const float SIFT_RATIO_THRESH = 0.75;
const float SURF_RATIO_THRESH = 0.7;
const float DET_THRESH = 0.1;
const int SURF_HESSIAN_THRESH = 400;

void check_similarity(vector<DMatch> matches, vector<KeyPoint> kp1, vector<KeyPoint> kp2){
    if ( matches.size() > MATCH_THRESHOLD){
        // If there are enough matches, estimate the homography matrix
        DMatch match;
        vector<Point2f> image1_points, image2_points;
        for (size_t index = 0; index < matches.size(); index++){
                match = matches[index];
                image1_points.push_back(kp1[match.queryIdx].pt);
                image2_points.push_back(kp2[match.trainIdx].pt);
        }

        Mat H;
        H = findHomography(image1_points, image2_points, RANSAC);
        
        float det = determinant(H);
        cout << "\tDeterminant: " << det << "\n";

        if (det > DET_THRESH){ // This does not work
            cout << "\tThe image contents are similar.\n";
        }
        else{
            cout << "\tThe image contents are processed by a strong transformation.\n";
        }
    }
    else{
        cout << "\tThe image contents are not similar.\n";
    }
}

int main( int argc, char** argv ){
    if (argc < 2 )
		return 0;
	
	Mat img1 = imread(argv[1], IMREAD_GRAYSCALE);
    Mat img2 = imread(argv[2], IMREAD_GRAYSCALE);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~ i) Evaluate features of both images ~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // ~~~~~~ ORB features ~~~~~~
    Ptr<FeatureDetector> orb_detector = ORB::create();
    vector< KeyPoint > orb_kp1, orb_kp2;
    Mat orb_des1, orb_des2;
    // Detect keypoints
    orb_detector->detect(img1, orb_kp1);
    orb_detector->detect(img2, orb_kp2);
    // Compute descriptors
    orb_detector->compute(img1, orb_kp1, orb_des1);
    orb_detector->compute(img2, orb_kp2, orb_des2);

    // ~~~~~~ SIFT features ~~~~~~
    Ptr<SiftFeatureDetector> sift_detector = SiftFeatureDetector::create();
    vector< KeyPoint> sift_kp1, sift_kp2;
    Mat sift_des1, sift_des2;
    // Detect keypoints
    sift_detector->detect(img1, sift_kp1);
    sift_detector->detect(img2, sift_kp2);
    // Compute descriptors
    sift_detector->compute(img1, sift_kp1, sift_des1);
    sift_detector->compute(img2, sift_kp2, sift_des2);

    // ~~~~~~ SURF features ~~~~~~
    Ptr<xfeatures2d::SURF> surf_detector = xfeatures2d::SURF::create( SURF_HESSIAN_THRESH );
    vector< KeyPoint> surf_kp1, surf_kp2;
    Mat surf_des1, surf_des2;
    // Detect keypoints
    surf_detector->detect(img1, surf_kp1);
    surf_detector->detect(img2, surf_kp2);
    // Compute descriptors
    surf_detector->compute(img1, surf_kp1, surf_des1);
    surf_detector->compute(img2, surf_kp2, surf_des2);
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~ ii) Match the features ~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Ptr<DescriptorMatcher> bf_matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE); // BF with L2 distance
    Ptr<DescriptorMatcher> bf_hamming_matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING); // BF with Hamming distance
    vector< vector<DMatch> > orb_knn_matches, sift_knn_matches, surf_knn_matches;
    
    // ~~~~~~ ORB matching ~~~~~~
    // Use Hamming distance on ORB since it is a binary string based descriptor
    bf_hamming_matcher->knnMatch( orb_des1, orb_des2, orb_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    vector<DMatch> orb_good_matches;
    for (size_t i = 0; i < orb_knn_matches.size(); i++)
    {
        if (orb_knn_matches[i][0].distance < ORB_RATIO_THRESH * orb_knn_matches[i][1].distance)
        {
            orb_good_matches.push_back(orb_knn_matches[i][0]);
        }
    }
    Mat orb_matches;
    drawMatches( img1, orb_kp1, img2, orb_kp2, orb_good_matches, orb_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("ORB Good Matches", orb_matches );
    imwrite("ORB.jpg", orb_matches);
    

    // ~~~~~~ SIFT matching ~~~~~~
    bf_matcher->knnMatch( sift_des1, sift_des2, sift_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    vector<DMatch> sift_good_matches;
    for (size_t i = 0; i < sift_knn_matches.size(); i++)
    {
        if (sift_knn_matches[i][0].distance < SIFT_RATIO_THRESH * sift_knn_matches[i][1].distance)
        {
            sift_good_matches.push_back(sift_knn_matches[i][0]);
        }
    }
    Mat sift_matches;
    drawMatches( img1, sift_kp1, img2, sift_kp2, sift_good_matches, sift_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("SIFT Good Matches", sift_matches );
    imwrite("SIFT.jpg", sift_matches);

    // ~~~~~~ SURF matching ~~~~~~
    bf_matcher->knnMatch( surf_des1, surf_des2, surf_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    std::vector<DMatch> surf_good_matches;
    for (size_t i = 0; i < surf_knn_matches.size(); i++)
    {
        if (surf_knn_matches[i][0].distance < SURF_RATIO_THRESH * surf_knn_matches[i][1].distance)
        {
            surf_good_matches.push_back(surf_knn_matches[i][0]);
        }
    }
    Mat surf_matches;
    drawMatches( img1, surf_kp1, img2, surf_kp2, surf_good_matches, surf_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("SURF Good Matches", surf_matches );
    imwrite("SURF.jpg", surf_matches);
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~ iii) Classify if                   ~~
    // ~~   Similar image content            ~~
    // ~~   Similar and strongly transformed ~~
    // ~~   Dissimilar                       ~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    cout << "ORB result:\n";
    check_similarity(orb_good_matches, orb_kp1, orb_kp2);
    cout << "SIFT result:\n";
    check_similarity(sift_good_matches, sift_kp1, sift_kp2);
    cout << "SURF result:\n";
    check_similarity(surf_good_matches, surf_kp1, surf_kp2);

    waitKey(0);

}