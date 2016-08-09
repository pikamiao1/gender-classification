#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>

#include "pcaImpl.h"
#include "myGabor.h"

using namespace cv;
using namespace std;

namespace bfs = boost::filesystem;
namespace ba  = boost::algorithm;
namespace bpt = boost::posix_time;


#define THROW_ERROR(err_code, err_msg)                 \
    {                                                  \
        ostringstream oss;                             \
        oss << err_msg << ", err_code = " << err_code; \
        throw runtime_error (oss.str ());              \
    }

// shared parameters
//---------------------------------------
#define FEMALE 1                // set female type to 1
#define MALE   2                // set male type to 2

#define MALE_SAMPLE_NO   340    //no. of male sample
#define FEMALE_SAMPLE_NO 340    //no. of female sample

#define MALE_TEST_NO     100
#define FEMALE_TEST_NO   100

#define kSizeX 5               // Gabor kernel scale
#define kSizeY 9

#define SIGMA 2*CV_PI
#define THETA_BOUND  8
#define LAMBDA_BOUND 5
#define GAMMA 1

#define PCA_RATE       1.0
#define PCA_FEATURE_NO 22
#define DESCRIPTOR_DIM PCA_FEATURE_NO*40

//const string in_type (".bmp");
const string out_type (".png");
const string face_cascade_file = "./haarcascade_frontalface_alt.xml";


// shared data type and functions
//---------------------------------------
Mat detect_face (Mat& frame);
float detect_gender_per_face (Mat& ROI);
