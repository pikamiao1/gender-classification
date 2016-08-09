#include "utils.h"


float detect_gender (Mat& frame) {

    CascadeClassifier face_cascade;

    if (!face_cascade.load (face_cascade_file)) {
        THROW_ERROR (-1, "cannot load classifier: " + face_cascade_file);
    }

    if (frame.data == 0) {
        THROW_ERROR (-1, "no image data in Mat");
    }

    // face detection
    Mat ROI = detect_face (frame);

    if (ROI.data == 0) {
        return -1;
    }

    float response = detect_gender_per_face (ROI);

    return response;
}


double cal_precision (const string in_dir,
                      int test_no,
                      int type) {

    // loop through file lists
    system (("rm -fr "+ in_dir + "/.DS_Store").c_str());

    bfs::path in_path (in_dir);

    vector<bfs::path> in_paths;
    copy (bfs::directory_iterator (in_path), bfs::directory_iterator(), back_inserter (in_paths));

    if (test_no > in_paths.size()) {
        THROW_ERROR (-1, "not enough training pictures in dir: "+ in_dir);
    }

    int trueCount = 0;
    int zeroCount = 0;

    for (int i = 0; i < test_no && i < in_paths.size(); ++i) {

        string in_file = in_paths[i].string();
        Mat in_img = imread (in_file, 0); //load gray scale

        float response = detect_gender (in_img);

        if (response == -1){
            zeroCount++;
        }
        else if (response == type) {
            trueCount++;
        }
        else {
          //do nothing
        }
        //cout << in_file << " " << response << endl;
    }

    double precision = (double) trueCount / (test_no - zeroCount);

    cout << "true: " << trueCount << endl;
    cout << "zeros: "<< zeroCount << endl;

    return precision;
}



int main (int argc, char *argv[]) {

    if (argc != 2 && argc != 3 ) {
        THROW_ERROR (argc, "wrong number of arguments");
        cout << "usage: ./test /path/to/test/folder/or/file [cal]" << endl;
    }

    if (argc == 3){

        bpt::ptime time_start = bpt::microsec_clock::local_time();

        if (ba::iequals (argv[2], "cal")) { // calculate precision

            const bfs::path in_path (argv[1]);

            if (! bfs::is_directory (in_path)) {
                THROW_ERROR (-1, in_path.string() + "is not a directory, need dir for precision evaluation");
            }

            cout << "calculating female precision" << endl;
            double female_pre = cal_precision (in_path.string() + "/female", FEMALE_TEST_NO, FEMALE); // female 1

            cout << "calculating male precision" << endl;
            double male_pre = cal_precision (in_path.string() + "/male", MALE_TEST_NO, MALE);   // male 2

            double overall_pre = (female_pre * FEMALE_TEST_NO + male_pre * MALE_TEST_NO) \
                                  / (FEMALE_TEST_NO + MALE_TEST_NO);

            // cout result
            cout << endl;
            cout << "type     \tfemale\tmale\toverall" << endl;

            cout << "total    \t"
                 << FEMALE_TEST_NO << "\t"
                 << MALE_TEST_NO << "\t"
                 << FEMALE_TEST_NO + MALE_TEST_NO << endl;

            cout << "success  \t"
                 << (int) (female_pre * FEMALE_TEST_NO)<< "\t"
                 << (int) ( male_pre  * MALE_TEST_NO) << "\t"
                 << (int) (female_pre * FEMALE_TEST_NO + male_pre * MALE_TEST_NO)<< endl;

            cout << "precision\t"
                 <<  (double) female_pre  * 100  << "\t"
                 <<  (double)   male_pre  * 100  << "\t"
                 <<  (double) overall_pre * 100  << "\t" << endl;

            cout << endl;
            cout << "------------------" << endl;
            cout << "kSizeX: " << kSizeX << endl;
            cout << "kSizeY: " << kSizeY << endl;
            cout << "PCA_FEATURE_NO: " << PCA_FEATURE_NO << endl;

            cout << endl;
            bpt::time_duration time_total = bpt::microsec_clock::local_time() - time_start;
            cout << "total evaluation time = " << time_total.total_microseconds() / 1000.0 / 1000.0 << " s" << endl;

            cout << endl;

        }
        else {
            THROW_ERROR (-1, "invalid command");
        }

        return 0;
    }

    else { // test single file

        bpt::ptime time_start = bpt::microsec_clock::local_time();

        const bfs::path in_path (argv[1]);

        if (! bfs::is_regular_file (in_path)) {
            THROW_ERROR (-1, in_path.string() + "is not a regular file, only support single image");
        }

        Mat test_img = imread (in_path.string(), 1); // load as color img
        int response = detect_gender (test_img);

        // put text
        if (response == 1) {
            cout << response << " female" << endl;
            //putText(test_img, "female", centerPoint, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 1.0);
        }
        else if (response == 2) {
            cout << response << " male" << endl;
            //putText(test_img, "male", centerPoint, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 1.0);
        }
        else {
            cout << "no face detected" << endl;
        }

        cout << endl;
        bpt::time_duration time_total = bpt::microsec_clock::local_time() - time_start;
        cout << "processing time = " << time_total.total_microseconds() / 1000.0 / 1000.0 << " s" << endl;

        cout << endl;

        // show result
        imshow ("face dectection", test_img);

        waitKey(0);
        system("pause");
    }
}

