#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(){

    Mat image;
    image = imread("test.bmp", CV_LOAD_IMAGE_UNCHANGED);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);   

      return 0;
}
