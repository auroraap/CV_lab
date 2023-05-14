#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

#include "resources/PanoramicUtils/header/panoramic_utils.h"
#include "lab6_resources/matcher.h"

using namespace std;
using namespace cv;

class Stitcher {
    private:
        string imagePath;
        string imageFiletype;
        int camAngle;
        Matcher matcher;
    public:
        Stitcher(string p, string f, int a, Matcher m) {
            imagePath = p;
            imageFiletype = f;
            camAngle = a;
            matcher = m;
        };
        vector< Mat > extractImagesOrdered();
        vector< Mat > projectImagesCylinder(vector< Mat > images);
        vector< Mat > getImageTranslation(vector< Mat > images);
};

vector< Mat > Stitcher::extractImagesOrdered(){
    vector< Mat > imageVec;
    vector<cv::String> filenames;
    glob(imagePath+"/*."+imageFiletype, filenames, false); // get filenames

    size_t imageCount = filenames.size();
    for (size_t i=0; i<imageCount; i++) {
        imageVec.push_back(imread(filenames[i]));
    }
        
    return imageVec;
}

vector< Mat > Stitcher::projectImagesCylinder(vector< Mat > images) {
    vector< Mat > projectedImages;
    for (int i = 0; i < images.size(); i++) {
        Mat proj = cylindricalProj(images[i], camAngle);
        projectedImages.push_back(proj);
    }
    return projectedImages;
}

vector< Mat > getImageTranslation(vector< Mat > images, Matcher matcher){
    vector< Mat > H_vec;

    for (int i = 0; i < images.size() - 1; i++){ // Iterate over all pairs
        Mat img1 = images[i];
        Mat img2 = images[i+1];
        vector< KeyPoint> kp1, kp2;
        Mat des1, des2;

        // Use matcher to find good matches between image 1 and 2
        matcher.buildDescriptor(img1, img2, &kp1, &kp2, &des1, &des2);
        vector<DMatch> good_matches = matcher.matchKeypoints(des1, des2);
        
        // From lab 6 check_similarity function:
        DMatch match;
        vector<Point2f> image1_points, image2_points;
        for (size_t index = 0; index < good_matches.size(); index++){
                match = good_matches[index];
                image1_points.push_back(kp1[match.queryIdx].pt);
                image2_points.push_back(kp2[match.trainIdx].pt);
        }

        Mat H;
        H = findHomography(image1_points, image2_points, RANSAC);

        // Add homography matrix to vector
        H_vec.push_back(H);
    }

    return H_vec;
}