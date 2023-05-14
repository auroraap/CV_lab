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
        vector< Mat > getImageTranslation(vector< Mat > images);
};