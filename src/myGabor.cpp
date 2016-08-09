#include "myGabor.h"


Mat myGabor::getKernel (float sigma,
                    	  float theta,
                    	  float lambda,
                        float gamma,
                    	  int kernelType,			//indicate real part or imaginary part
                    	  Size kernelSize)
{
  // v: 代表波长
  // u: 代表方向
  // Kmax=2.5*pi/2, f=sqrt(2), sigma=1.5*pi;
  // U,方向因子{0,1,2,3,4,5,6,7}
  // V,大小因子{0,1,2,3,4}

	int xMax = ceil(max(1, kernelSize.width/2));
	int yMax = ceil(max(1, kernelSize.height/2));

  //Qu = pi*U/8;  theta
	Mat kernel(yMax*2+1, xMax*2+1, CV_32F);

	//float *rRows, *iRows;
	float *rows;
	float pi = CV_PI;
	float sigmaSq = sigma * sigma;
	float direction = pi * theta / 8 + pi/2; // input of theta: {0, 1, 2, 3, 4, 5, 6, 7, 8}
  //kv = 2.5 * (pi / 2)/pow( 2, -lambda/2); // input of lambda: {0, 1, 2, 3, 4}

	float kv = 2.5 * pi * pow(2, -(lambda+2)/2);

	float lastExp = exp(-sigmaSq/2);

	//cout << lastExp << endl;

	for(int y = 0; y <= yMax*2; y++)
	{

		rows = kernel.ptr<float>(y);


		for(int x = 0; x <= xMax*2; x++)
		{
			int i = x - xMax;
			int j = y - yMax;
			//cout << "y:" << y << endl;
			//cout << "x:" << x << endl;
			float firstExp = exp(-(kv * kv * (i*i + j*j) / (2 * sigmaSq)));
			//float rPartExp = cos(kv * cos(direction) * x + kv * sin(direction) * y) - lastExp;
			//float iPartExp = sin(kv * cos(direction) * x + kv * sin(direction) * y);
			float rPartExp = cos(kv * cos(direction) * i + kv * sin(direction) * j) - lastExp;
			float iPartExp = sin(kv * cos(direction) * i + kv * sin(direction) * j);

			if(kernelType == 0){
				rows[x] = kv * kv * firstExp * rPartExp / sigmaSq;
			}
			else{
				rows[x] = kv * kv * firstExp * iPartExp / sigmaSq;
			}
		}
	}

	return kernel;
}


Mat myGabor::filterImg (Mat& src,Mat& kernel)
{
    Mat outImg;
    filter2D(src, outImg, CV_32F, kernel, Point(-1,-1), 0, BORDER_REPLICATE);

    return outImg;
}

Mat myGabor::filterImg (Mat& src, Mat& kernel, int filterType)
{
	  Mat outImg;
    filter2D(src, outImg, CV_32F, kernel, Point(-1,-1), 0, BORDER_REPLICATE);

    return outImg;
}


Mat myGabor::calculateMagnitude (Mat &realImg,Mat &imgnrImg)
{
    int row = realImg.rows;
    int col = realImg.cols;

    Mat outImg = Mat::zeros(row, col, CV_32F);

    pow(realImg, 2, realImg);
    pow(imgnrImg, 2, imgnrImg);

    sqrt( (imgnrImg + realImg) ,outImg);

    return outImg;
}


Mat myGabor::getGaborImg (Mat& src,
            						  float sigma,
            						  float theta,
            						  float lambda,
            						  float gamma,
            						  Size kernelSize )
{
    Mat rPartKernel = getKernel( sigma, theta, lambda, gamma, 0, kernelSize);
    Mat iPartKernel = getKernel( sigma, theta, lambda, gamma, 1, kernelSize);
	  //imshow("rPartKernel", rPartKernel);
	  //imshow("iPartKernel", iPartKernel);

    Mat rPartImg = filterImg(src, rPartKernel);
  	Mat iPartImg = filterImg(src, iPartKernel);
  	//imshow("r Part", rPartImg);
  	//imshow("i Part", iPartImg);
  	return calculateMagnitude(rPartImg, iPartImg);
}
