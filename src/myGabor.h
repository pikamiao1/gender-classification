#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class myGabor
{
	public:
		static Mat getGaborImg (Mat& src,
                                float sigma,
                                float theta,
                                float lambda,
                                float gamma,
                                Size kernelSize);

   	static Mat getKernel (float sigma,
                          float theta,
                          float lambda,
                          float gamma,
                          int kernelType,
                          Size kernelSize);

    static Mat filterImg (Mat& src,Mat& kernel);

    static Mat filterImg (Mat& src, Mat& kernel, int filterType);

    static Mat calculateMagnitude (Mat &realImg,Mat &imgnrImg);
};