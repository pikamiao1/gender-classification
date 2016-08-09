#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

class pcaImpl
{
public:
    pcaImpl (Mat& _data, int maxComponents=0, float retainedVariance = 1.0);
    Mat pcaProject(Mat _data);
    Mat pcaInverseProject(Mat _data);



private:

    Mat meanMat;            // should be 1-D matrix
    Mat covarMat;
    Mat eigenValues;
    Mat eigenVectors;

    pcaImpl& init(Mat& _data, int maxComponents, float retainedVariance);
    template <typename T>
    int computeCumEnergy(const Mat& eigenvalues, T retainedVariance);

};
