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