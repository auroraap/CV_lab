// main.cpp

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define K 3

int main( int argc, char** argv ){
    if (argc < 2 )
		return 0;
	
	Mat img1 = imread(argv[1]);
    Mat img2 = imread(argv[2]);

    // i) Evaluate features of both images
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
    orb_des1.convertTo(orb_des1, CV_32F);
    orb_des2.convertTo(orb_des2, CV_32F);

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
    sift_des1.convertTo(sift_des1, CV_32F);
    sift_des2.convertTo(sift_des2, CV_32F);

    // ~~~~~~ SURF features ~~~~~~
    int hessian = 600;
    Ptr<xfeatures2d::SURF> surf_detector = xfeatures2d::SURF::create( hessian );
    vector< KeyPoint> surf_kp1, surf_kp2;
    Mat surf_des1, surf_des2;
    // Detect keypoints
    surf_detector->detect(img1, surf_kp1);
    surf_detector->detect(img2, surf_kp2);
    // Compute descriptors
    surf_detector->compute(img1, surf_kp1, surf_des1);
    surf_detector->compute(img2, surf_kp2, surf_des2);
    surf_des1.convertTo(surf_des1, CV_32F);
    surf_des2.convertTo(surf_des2, CV_32F);
    
    // ii) Match the features
    Ptr<DescriptorMatcher> flann_matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > orb_knn_matches, sift_knn_matches, surf_knn_matches;
    const float ratio_thresh = 0.8f;

    // ~~~~~~ ORB matching ~~~~~~
    flann_matcher->knnMatch( orb_des1, orb_des2, orb_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    std::vector<DMatch> orb_good_matches;
    for (size_t i = 0; i < orb_knn_matches.size(); i++)
    {
        if (orb_knn_matches[i][0].distance < ratio_thresh * orb_knn_matches[i][1].distance)
        {
            orb_good_matches.push_back(orb_knn_matches[i][0]);
        }
    }
    Mat orb_matches;
    drawMatches( img1, orb_kp1, img2, orb_kp2, orb_good_matches, orb_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("ORB Good Matches", orb_matches );
    

    // ~~~~~~ SIFT matching ~~~~~~
    flann_matcher->knnMatch( sift_des1, sift_des2, sift_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    std::vector<DMatch> sift_good_matches;
    for (size_t i = 0; i < sift_knn_matches.size(); i++)
    {
        if (sift_knn_matches[i][0].distance < ratio_thresh * sift_knn_matches[i][1].distance)
        {
            sift_good_matches.push_back(sift_knn_matches[i][0]);
        }
    }
    Mat sift_matches;
    drawMatches( img1, sift_kp1, img2, sift_kp2, sift_good_matches, sift_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("SIFT Good Matches", sift_matches );

    // ~~~~~~ SURF matching ~~~~~~
    flann_matcher->knnMatch( surf_des1, surf_des2, surf_knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    std::vector<DMatch> surf_good_matches;
    for (size_t i = 0; i < surf_knn_matches.size(); i++)
    {
        if (surf_knn_matches[i][0].distance < ratio_thresh * surf_knn_matches[i][1].distance)
        {
            surf_good_matches.push_back(surf_knn_matches[i][0]);
        }
    }
    Mat surf_matches;
    drawMatches( img1, surf_kp1, img2, surf_kp2, surf_good_matches, surf_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow("SURF Good Matches", surf_matches );
    waitKey(0);

    // iii) Classify if
        // Similar image content
        // Similar but strongly transformed
        // Dissimilar

}