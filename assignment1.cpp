#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>


using namespace cv;
using namespace std;
int main()
{
    VideoCapture cap;
    vector < Point2f> corners2d;
    vector < Point3f> corners3d;
    vector < Point2f> project2d;
    vector <Point3f> project;
    vector <Point3f> axis;
    vector <Point2f> axis2d;


    float l = 2.5;  //lenght of a square on the chessboard
    float k = 0.0;  //used for moving the cube left or right
    bool m = 0;      //move the cube left or right


    //reading camera matrix and distortion from yml file after calibration

    FileStorage fs("C:/Users/trekk/Desktop/Assingment1/calibration/camera.yml", FileStorage::READ); //has to be modified with the location of the calibration file
    Mat intrinsics, distortion;
    fs["camera_matrix"] >> intrinsics;
    fs["distortion_coefficients"] >> distortion;

    //explicit camera matrix

    cout << intrinsics;

    if (!cap.open(0))
        return 0;
   


        for (;;)
        {
            Mat frame;
            Mat tvec,rvec;
            cap >> frame;

            //calculating the coordinates of the inner corners of the chessboard (imagepoints)

            bool found = findChessboardCorners(frame, Size(9, 6), corners2d, CALIB_CB_ADAPTIVE_THRESH);

            if (found) {


                    //creating a vector containing all the points of the cube
                    
                    project.push_back(Point3f(0 +k/10, 0, 0));   //7
                    project.push_back(Point3f(0 + k / 10, l, 0));   //6
                    project.push_back(Point3f(l + k / 10, l, 0)); //5
                    project.push_back(Point3f(l + k / 10, 0, 0));  //4

                    project.push_back(Point3f(0 +k/10, 0, -l));   //3
                    project.push_back(Point3f(0 + k / 10,  l, -l));   //2
                    project.push_back(Point3f( l + k / 10,  l, -l)); //1
                    project.push_back(Point3f( l + k / 10, 0, -l));  //0

                    //Points of the axis

                    axis.push_back(Point3f(0, 0, -2 * l)); //2
                    axis.push_back(Point3f(0, 2 * l, 0));   //1
                    axis.push_back(Point3f(2 * l, 0, 0));   //0


                    //creating the vector containing the object points

                    for (int y = 0; y < 6; ++y) {

                        for (int x = 0; x < 9; ++x) {

                            corners3d.push_back(cv::Point3f(x * 2.5, y * 2.5, 0));

                        }

                    }

                    

                    //calculate rvec and tvec

                    solvePnP(corners3d, corners2d, intrinsics, distortion, rvec, tvec, false);

                    //get positions of the points of the 3d cube relative to the rvec and tvec

                    projectPoints(project, rvec, tvec, intrinsics, distortion, project2d);

                    //get positions of the points of the axis lines relative to the rvec and tvec

                    projectPoints(axis, rvec, tvec, intrinsics, distortion, axis2d);



                    //draw axis

                    line(frame, corners2d[0], axis2d[0], Scalar(255, 0, 0), 2);
                    line(frame, corners2d[0], axis2d[1], Scalar(0, 255, 0), 2);
                    line(frame, corners2d[0], axis2d[2], Scalar(0, 0, 255), 2);


                    //draw bottom square face

                    line(frame, project2d[4] , project2d[5] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[5] , project2d[6] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[6] , project2d[7] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[7] , project2d[4] , Scalar(0, 0, 255), 3);

                    //draw vertical square edges

                    line(frame, project2d[0] , project2d[4] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[1] , project2d[5] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[2] , project2d[6] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[3] , project2d[7] , Scalar(0, 0, 255), 3);

                    //draw top square face

                    line(frame, project2d[0] , project2d[1] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[1] , project2d[2] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[2] , project2d[3] , Scalar(0, 0, 255), 3);
                    line(frame, project2d[3] , project2d[0] , Scalar(0, 0, 255), 3);

                    
                    //slide the cube across the x axis by incrementing x value of the points of the cube

                    if (k == 0)
                        m = 1;
                    if (k == 100)
                        m = 0;
                    if (m == 0)
                        k--;
                    if (m == 1)
                        k++;
                        
                       
                }

                imshow("image", frame);


                corners2d.clear();
                corners3d.clear();
                project.clear();
                project2d.clear();
                axis.clear();
                axis2d.clear();

               

                if (waitKey(10) == 27) break; //ending the camera capture
            
        }

        return 0;

    }
