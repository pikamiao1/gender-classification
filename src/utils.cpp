#include "utils.h"


Mat detect_face (Mat& frame) {

    // grayscale
    Mat frame_gray = frame;

    if (frame.channels() > 1){
        cvtColor (frame, frame_gray, CV_BGR2GRAY);
    }
    equalizeHist (frame_gray, frame_gray);

    // detect
    CascadeClassifier face_cascade;

    if (!face_cascade.load (face_cascade_file)) {
        THROW_ERROR (-1, "cannot load classifier: " + face_cascade_file);
    }

    std::vector<Rect> faces;
    face_cascade.detectMultiScale (frame_gray, faces,
                                   1.1, 2, 0|CV_HAAR_SCALE_IMAGE,
                                   Size(30, 30) );

    Mat ROI;

    for( int i = 0; i < faces.size(); i++ )
    {
        Point center (faces[i].x + faces[i].width*0.5,
                      faces[i].y + faces[i].height*0.5 );

        // extrace face area
        ROI = frame_gray (Rect (faces[i].tl().x,
                                    faces[i].tl().y,
                                    faces[i].width,
                                    faces[i].height));

        resize (ROI, ROI, Size(21,18), 0, 0, CV_INTER_LINEAR);

        // draw rectangle on original image
        rectangle (frame,
                   faces[i].tl(), faces[i].br(),
                   Scalar(0, 255, 0), 3);
    }

    return ROI; //this casue return of last face onlym, change to vector later
}



float detect_gender_per_face (Mat& ROI){

    // gabor feature
    normalize (ROI, ROI, 1, 0, CV_MINMAX, CV_32F);

    Mat feature_Gabor;

    for (int i=0;i<THETA_BOUND;i++) {

        for (int j=0;j<LAMBDA_BOUND;j++) {

            Mat M_Magnitude = myGabor::getGaborImg (ROI,
                                                    SIGMA,    //sigma
                                                    i,        //theta
                                                    j,        //lambda
                                                    GAMMA,    //gamma
                                                    Size(kSizeX, kSizeY)); //kernel size

            normalize (M_Magnitude, M_Magnitude, 0, 1, CV_MINMAX,CV_32F);

            Mat feature_line= M_Magnitude.reshape (0,1);
            feature_Gabor.push_back (feature_line);
        }
    }

    // pca
    pcaImpl pca = pcaImpl::pcaImpl (feature_Gabor, PCA_FEATURE_NO, PCA_RATE);
    Mat dst = pca.pcaProject (feature_Gabor) ;
    //cout << "dst:" << dst.size() <<endl;

    Mat feature_line = dst.reshape (0,1);
    //cout << "feature_line: " << line.size() <<endl;

    // svm classification
    CvSVM mySVM;
    mySVM.load ("SVM_PCA.xml");
    float response = mySVM.predict (feature_line );

    feature_Gabor.release();

    return response;
}

