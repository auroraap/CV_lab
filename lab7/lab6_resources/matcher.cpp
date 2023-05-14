#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Matcher {
    private:
        int match_threshold;
        float ratio_threshold;
        int K;
        float determinant_threshold;
    public:
        Ptr<FeatureDetector> detector;
        Ptr<DescriptorMatcher> matcher;
        void setVars(int match_threshold, float ratio_threshold, int K, float determinant_th);
        void buildDescriptor(Mat img1, Mat img2, vector< KeyPoint>* kp1,  vector< KeyPoint>* kp2, Mat* des1, Mat* des2);
        void checkSimilarity( vector<DMatch> good_matches , vector< KeyPoint> kp1, vector< KeyPoint> kp2 );
        vector<DMatch> matchKeypoints(Mat des1, Mat des2);
};

void Matcher::setVars(int match_th, float ratio_th, int k, float determinant_th){
    match_threshold = match_th;
    ratio_threshold = ratio_th;
    K = k;
    determinant_threshold = determinant_th;
}

void Matcher::buildDescriptor(Mat img1, Mat img2, vector< KeyPoint>* kp1,  vector< KeyPoint>* kp2, Mat* des1, Mat* des2){
    Mat descriptor1, descriptor2;
    vector< KeyPoint> keypoint1, keypoint2;
    // Detect keypoints
    detector->detect(img1, keypoint1);
    detector->detect(img2, keypoint2);
    // Compute descriptors
    detector->compute(img1, keypoint1, descriptor1);
    detector->compute(img2, keypoint2, descriptor2);
    
    *kp1 = keypoint1;
    *kp2 = keypoint2;
    *des1 = descriptor1;
    *des2 = descriptor2;
}

vector<DMatch> Matcher::matchKeypoints( Mat des1, Mat des2 ){
    vector< vector<DMatch> > knn_matches;
    vector<DMatch> good_matches;
    matcher->knnMatch( des1, des2, knn_matches, K );
    // Eliminate false matches by using Lowe's distance ratio test
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_threshold * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    return good_matches;
}

void Matcher::checkSimilarity( vector<DMatch> good_matches , vector< KeyPoint> kp1, vector< KeyPoint> kp2 ){
    if ( good_matches.size() > match_threshold){
        // If there are enough matches, estimate the homography matrix
        DMatch match;
        vector<Point2f> image1_points, image2_points;
        for (size_t index = 0; index < good_matches.size(); index++){
                match = good_matches[index];
                image1_points.push_back(kp1[match.queryIdx].pt);
                image2_points.push_back(kp2[match.trainIdx].pt);
        }

        Mat H;
        H = findHomography(image1_points, image2_points, RANSAC);
        
        float det = determinant(H);
        cout << "\tDeterminant: " << det << "\n";

        if (det > determinant_threshold){ // This does not work
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