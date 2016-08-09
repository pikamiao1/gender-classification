#include "pcaImpl.h"


    /*
    * Compute the principal component of a given matrix.
    * We assume that the matrix regard every row as a sample
      eg -------------> y
        |
        |
        |
        x                for this(x,y) matrix, we have x rows/samples with y dimensions

    *
    // _data: Assuming the input data is recorded as ROW
    // maxComponents：required max dimension, must be less than sample's dimension (default = 0, means keep all)
    // retainedVariance: automatically calculation according to the cumulative percentage
    */
pcaImpl::pcaImpl(Mat& _data,int maxComponents, float retainedVariance)
{
    pcaImpl::init(_data, maxComponents, retainedVariance);
}

pcaImpl& pcaImpl::init(Mat& _data, int maxComponents, float retainedVariance)
{

    Mat data = _data.clone();
    // cout<<"data -->"<<data<<endl;
    // Mat data = _data.getMat();
    int initDim = data.cols;         // initial dimension of data
    int dataCount = data.rows;          // number of data samples
    int ctype = std::max(CV_32F, data.depth());

    // 1. Compute the mean of every dimension
    meanMat = Mat(1, initDim, ctype, Scalar::all(0));
    for (int i = 0; i < initDim; i++){
        meanMat.at<float>(0,i) = (float) sum(data.col(i))[0] / dataCount;   //compute dim mean
    }
    // cout<<"meanMat -->"<<meanMat<<endl;

    // 2. Subtract mean image from the data set to get mean centered data vector
    Mat tmpMeanMat = repeat(meanMat, dataCount, 1);
    data = data - tmpMeanMat;

    // 3. Compute the covariance matrix from the mean centered data matrix
    //先让样本矩阵中心化，即每一维度减去该维度的均值，使每一维度上的均值为0，然后直接用新的到的样本矩阵乘上它的转置，然后除以(N-1)即可
    covarMat = Mat(initDim, initDim, ctype);        // the covarMat is a squear matrix, dim == sample's dim
    covarMat = (data.t()*data) / (dataCount - 1);   // （X'*X)/n-1
    // cout<<"covarMat -->"<<covarMat<<endl;

    // 4. Calculate the eigenvalues and eigenvectors for the covariance matrix
    eigen( covarMat, eigenValues, eigenVectors );

    // 5. Normalize the eigen vectors
    for(int i = 0; i < initDim; i++ )
    {
        Mat vec = eigenVectors.row(i);          //Passed by reference
        normalize(vec, vec);
    }

    // 6. Find out an eigenvector with the largest eigenvalue which distingushes the data
    int count = std::min(initDim, dataCount), out_count = count;
    //compute the cumulative energy content for each eigenvector
    int remainCount = 0;
    if(retainedVariance<1.0)
        if (ctype == CV_32F)
            remainCount = computeCumEnergy<float>(eigenValues, retainedVariance);
        else
            remainCount = computeCumEnergy<double>(eigenValues, retainedVariance);
    if(remainCount > 0)
        out_count = std::min(count, remainCount);
    else if( maxComponents > 0 )        //constrain the max dimensions
        out_count = std::min(count, maxComponents);
    // cout<<"outCount of PCA : "<<out_count<<endl;

    eigenValues = eigenValues.rowRange(0, out_count).clone();
    eigenVectors = eigenVectors.rowRange(0,out_count).clone();

    // cout << "eigenValues-->"<<eigenValues << endl;
    // cout << "eigenVectors-->"<<eigenVectors << endl;

    return *this;

}


Mat pcaImpl::pcaProject(Mat _data)
{
    Mat result;
    Mat data = _data.clone();
    Mat tmpMeanMat = repeat(meanMat, data.rows/meanMat.rows, data.cols/meanMat.cols);
    subtract( data, tmpMeanMat, data);

    if(meanMat.rows == 1){
        gemm( data, eigenVectors, 1, Mat(), 0, result, GEMM_2_T );
    }
    // cout << "result-->"<<result << endl;
    return result;
}

Mat pcaImpl::pcaInverseProject(Mat _data)
{
    Mat result;
    Mat data = _data.clone();

    if(meanMat.rows == 1){
        Mat tmpMeanMat = repeat(meanMat, data.rows/meanMat.rows,1);
        gemm( data, eigenVectors, 1, tmpMeanMat, 1, result, 0 );
    }

    return result;
}



template <typename T>
int pcaImpl::computeCumEnergy(const Mat& eigenValues, T retainedVariance){
    int eigenSize = eigenValues.rows;
    std::vector<T> vecEnergy;
    T sum = 0;
    for(int i = 0; i < eigenSize; i++)
    {
        sum += (T)eigenValues.at<T>(i,0);
        vecEnergy.push_back(sum);
    }
    // copy (vecEnergy.begin(), vecEnergy.end(), ostream_iterator<int> (cout, "\n"));
    int index;
    for(index = 0; index < eigenSize; index++)
    {
        T energy = (T)vecEnergy[index]/sum;
        if(energy > retainedVariance)
            break;
    }
    index = std::max(2, index);
    return index;
}


