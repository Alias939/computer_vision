#include <cstdlib>
#include <string>

#include "utilities/General.h"
#include "VoxelReconstruction.h"
#include "Camera.h"
#include <opencv2/opencv.hpp>

using namespace nl_uu_science_gmt;
using namespace std;
using namespace cv;

int main(
		int argc, char** argv)
{
    /*Mat foreground, background, result;
    foreground=imread("C:/Users/trekk/source/repos/Assignment2/data/cam1/foreground.png");
    background=imread("C:/Users/trekk/source/repos/Assignment2/data/cam1/background2.png");
    
    bitwise_and(foreground, background, result);
    
    imshow("result", result);
    waitKey(0);*/

   /* VideoCapture cap("C:/Users/trekk/source/repos/Assignment2/data/cam1/video.avi");
    if (!cap.isOpened())
    {
        return false;
    }
    Mat frame;
    cap >> frame;
    imwrite("C:/Users/trekk/source/repos/Assignment2/data/cam1/video.png", frame);*/

   

	VoxelReconstruction::showKeys();
	VoxelReconstruction vr("data" + std::string(PATH_SEP), 4);
	vr.run(argc, argv);

	return EXIT_SUCCESS;
}

void calibration() {
   
    vector <Point2f> imagepoints;

    for (size_t i = 1; i < 5; i++)
    {
        int n = 0;
        VideoCapture cap("data/cam" + std::string(to_string(i)) + std::string("/intrinsics.avi"));
        FileStorage fs("data/cam" + std::string(to_string(i)) + std::string("/calibrate") + std::string("/imagelist.xml"), FileStorage::WRITE);
        fs << "images" << "[";
        while (1) {

            Mat frame;
            cap >> frame;

            // If the frame is empty, break immediately
            if (frame.empty())
                break;

            bool found = findChessboardCorners(frame, Size(8, 6), imagepoints, CALIB_CB_FAST_CHECK);

            if (found) {

                imwrite("data/cam" + std::string(to_string(i)) + std::string("/calibrate") + std::string("/image") + std::string(to_string(n)) + std::string(".jpg"), frame);


                fs << "image" + std::string(to_string(n)) + std::string(".jpg");

                imshow("Frame", frame);

                n++;
                cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) + 50);
                if (n > 50)
                    break;


            }
            // Press  ESC on keyboard to exit
            char c = (char)waitKey(25);
            if (c == 27)
                break;

        }
        fs << "]";
    }
}
