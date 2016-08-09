#include "utils.h"


// string classifyGender(Mat _ROI);
Mat detectAndDisplay_FaceAndGender(Mat frame);

CascadeClassifier face_cascade;
string window_name = "Camera Gender Recognition";

int maleCount = 0;
int femaleCount = 0;

int main(){

    /*****Initialize the camera to capture images*****/
    VideoCapture cap(0);        //open the default camera
    if (!cap.isOpened()){
        printf("--(!)The camera has not been turned on, please check again\n");
        getchar();
        return -1;
    }
    /*****Initialized loading of the cascades*****/
    if (!face_cascade.load(face_cascade_file)) {
        printf("--(!)Error in loading face cascade file\n");
        getchar();
        return -1;
    };

    namedWindow(window_name, 1);
    //preprocessing image from BGR to Gray
    for (;;) {
        Mat frame;
        Mat frame_gray;
        cap >> frame;       //get a new frame from camera

        /*****Detect face and gender*****/
        detectAndDisplay_FaceAndGender(frame);

        /*****Show what we detected*****/
        imshow(window_name, frame);
        cvWaitKey(1);       // delay to show
    }
    return 0;
}


Mat detectAndDisplay_FaceAndGender(Mat frame)
{
    // 1. image preprocessing
    Mat frame_gray,ROI;
    if(frame.channels() > 1){
        cvtColor( frame, frame_gray, CV_BGR2GRAY );
    }
    equalizeHist( frame_gray, frame_gray );

    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( int i = 0; i < faces.size(); i++ )
    {
        // 2. detect face and add a rectangle on the face over the frame
        Point centerPoint( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        Point tlPoint(faces[i].tl().x, faces[i].tl().y);
        Point brPoint(faces[i].br().x, faces[i].br().y);

        rectangle(frame, faces[i], Scalar(255, 0, 255), 4, 8, 0);       //recOfFace is added on the "frame"

        //3. preprocession of the face area (ROI)
        ROI=frame_gray(Rect(tlPoint.x,tlPoint.y,faces[i].width,faces[i].height));
        resize(ROI,ROI,Size(21,18),0,0,CV_INTER_LINEAR);

        //4. detect face gender at ROI of per face
        float response = detect_gender_per_face (ROI);

        //5. show info of per face
        string genderResult = "";
        if(response==MALE){
            cout<<"male : "<<++maleCount<<endl;
            genderResult = "male";
        }else if(response==FEMALE){
            cout<<"female : "<<++femaleCount<<endl;
            genderResult = "female";
        }else{
            cout << "no face detected" << endl;
            genderResult = "failed to detect gender";
        }
        string faceInfoText = "Gender is " + genderResult;
        putText(frame, faceInfoText, centerPoint, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 1.0);
        //infoOfFace is added on the "frame"
    }

    // imshow( "window_name", frame );
    return frame;
}
