#include "utils.h"

void run_face_det (const string in_dir,
                   const string out_dir) {

    // make out_dir
    if (bfs::exists (out_dir)) {
        system (("rm -fr " + out_dir).c_str());
    }
    system (("mkdir -p " + out_dir).c_str());

    // loop through file lists
    system (("rm -fr "+ in_dir + "/.DS_Store").c_str());

    bfs::path in_path (in_dir);

    vector<bfs::path> in_paths;
    copy (bfs::directory_iterator (in_path), bfs::directory_iterator(), back_inserter (in_paths));

    for (int i = 0; i < in_paths.size(); ++i) {

        string in_file = in_paths[i].string();
        cout << "processing: " << in_file << endl;

        Mat in_img = imread (in_file, 1);

        if (in_img.data == 0) {
            THROW_ERROR (-1, "no image data in Mat: "+ in_file);
        }

        // detect face area
        Mat ROI = detect_face (in_img);

        stringstream ss;
        ss << out_dir
           << "/face_"
           << setfill('0') << setw(4) << (i+1)
           << out_type;

        // save face area to file
        imwrite(ss.str(), ROI);
    }
}


int run_gabor_pca (const string in_dir,
                   Mat& sample_feature,
                   Mat& sample_label,
                   int offset,
                   int sample_no,
                   int sample_type) {

    // loop through file lists
    system (("rm -f "+ in_dir + "/.DS_Store").c_str());

    bfs::path in_path (in_dir);

    vector<bfs::path> in_paths;
    copy (bfs::directory_iterator (in_path), bfs::directory_iterator(), back_inserter (in_paths));

    if (sample_no > in_paths.size()){
        THROW_ERROR (-1, "sample_no exceeds total faces under dir: "+ in_dir);
    }

    int trueCount = 0;

    for (int i = 0; i < sample_no && i < in_paths.size(); ++i) {

        string in_file = in_paths[i].string();
        cout << "training: " << in_file << endl;

        Mat ROI = imread (in_file, 0); // 0 means load grayscale

        if(ROI.data != 0) {
            normalize (ROI, ROI, 1, 0, CV_MINMAX, CV_32F);

            // Gabor feature extraction
            Mat Gabor_feature;

            for (int i=0;i<THETA_BOUND;i++) {

                for (int j=0;j<LAMBDA_BOUND;j++) {

                    Mat M_Magnitude = myGabor::getGaborImg (ROI,
                                                            SIGMA,    //sigma
                                                            i,        //theta
                                                            j,        //lambda
                                                            GAMMA,    //gamma
                                                            Size(kSizeX, kSizeY)); //kernel size

                    normalize (M_Magnitude, M_Magnitude, 0, 1, CV_MINMAX, CV_32F);

                    Mat feature_line = M_Magnitude.reshape (0,1);
                    Gabor_feature.push_back (feature_line);
                }
            }

            // run pca
            pcaImpl pca = pcaImpl::pcaImpl (Gabor_feature, PCA_FEATURE_NO, PCA_RATE);

            Mat dst = pca.pcaProject (Gabor_feature);
            Mat feature_line = dst.reshape (0,1);

            //将计算好的Gabor降维后描述子复制到样本特征矩阵sampleFeatureMat
            for (int i = 0; i < DESCRIPTOR_DIM; i++) {
                sample_feature.at<float> (trueCount + offset, i) = feature_line.at<float> (0,i);
            }
            sample_label.at<float> (trueCount + offset, 0) = sample_type;

            trueCount++;
            Gabor_feature.release();
        }
    }

    return trueCount;
}



int main (int argc, char *argv[]) {

    if (argc != 2 && argc != 3) {
        THROW_ERROR (argc, "wrong number of arguments");
        cout << "usage: ./train /path/to/training/folder [det]" << endl;
    }

    const bfs::path train_dir (argv[1]);
    const bfs::path output_dir (train_dir.string() + "_output");

    bool run_detection = false;

    if (argc == 3){
        if (ba::iequals (argv[2], "det") ) {
            run_detection = true;
        }
        else {
            THROW_ERROR (-1, "invalid command");
        }
    }

    if (! bfs::exists (output_dir.string())){
        run_detection = true;
    }

    cout << "start training" << endl;
    cout << endl;


    //=========
    // face_det (optional)
    //=========

    if (run_detection) {

        bpt::ptime time_start = bpt::microsec_clock::local_time();

        cout << "runing face detection" << endl;
        cout << "face output placed in " << output_dir.string() << endl;

        run_face_det (train_dir.string() + "/female", output_dir.string() + "/female" );
        run_face_det (train_dir.string() + "/male", output_dir.string() + "/male" );

        cout << endl;

        bpt::time_duration time_total = bpt::microsec_clock::local_time() - time_start;
        cout << "Total detection time = " << time_total.total_microseconds() / 1000.0 / 1000.0 << " s" << endl;

        cout << endl;
    }


    //============
    // gabor + pca
    //============

    bpt::ptime time_start = bpt::microsec_clock::local_time();

    int offset = 0;
    Mat sampleFeatureMat = Mat::zeros(MALE_SAMPLE_NO + FEMALE_SAMPLE_NO, DESCRIPTOR_DIM, CV_32FC1);
    Mat sampleLabelMat = Mat::zeros(MALE_SAMPLE_NO + FEMALE_SAMPLE_NO, 1, CV_32FC1);

    // train female first
    cout << "training female samples" << endl;

    offset += run_gabor_pca (output_dir.string() + "/female",
                             sampleFeatureMat,
                             sampleLabelMat,
                             offset,
                             FEMALE_SAMPLE_NO,
                             FEMALE);       // female 1
    cout << endl;

    // train male next
    cout << "training male samples" << endl;

    offset += run_gabor_pca (output_dir.string() + "/male",
                             sampleFeatureMat,
                             sampleLabelMat,
                             offset,
                             MALE_SAMPLE_NO,
                             MALE);       // male 2
    cout << endl;

    // remove non-zero fields
    int nonZeros = cv::countNonZero(sampleLabelMat);

    Mat nSampleLabels;
    Mat nSampleFeatures;

    sampleLabelMat (Range(0, nonZeros),Range(0,sampleLabelMat.cols)).copyTo(nSampleLabels);
    sampleFeatureMat (Range(0, nonZeros),Range(0,sampleFeatureMat.cols)).copyTo(nSampleFeatures);


    //=======
    // svm
    //=======

    cout << "training svm" << endl;

    //SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01
    //迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
    CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    CvSVMParams param (CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);

    CvSVM svm;
    svm.train (nSampleFeatures, nSampleLabels, Mat(), Mat(), param);
    svm.save ("SVM_PCA.xml");

    cout << endl;

    bpt::time_duration time_total = bpt::microsec_clock::local_time() - time_start;
    cout << "Total training time = " << time_total.total_microseconds() / 1000.0 / 1000.0 << " s" << endl;

    cout << endl;

    return 0;
}
