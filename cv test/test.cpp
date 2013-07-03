#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <vector>
#include <stdio.h>
#include <opencv/cv.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture capture;

    if (argc == 2)
        capture.open(argv[1]);  // Open file
    else
        capture.open(0);        // Open camera device

    if (!capture.isOpened())
    {
        cout << "Cannot open video device or file!" << endl;
        return -1;
    }

    Mat frame;
    namedWindow("video", CV_WINDOW_AUTOSIZE);
    while(true)
    {
        capture >> frame;
        if (frame.empty())
            break;

			
	
Mat hsv;                                          //HSV file of the frame
cvtColor(frame, hsv, CV_BGR2HSV);
Mat bw;
inRange(hsv, Scalar(150, 100, 40), Scalar(180, 255, 255), bw);       //tune and obtain the balls color alone
vector<vector<Point> > contours;
findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);  //find contourd
Mat dst = Mat::zeros(frame.size(), frame.type());
drawContours(dst, contours, -1, Scalar::all(255), CV_FILLED);  //draw contours
int q1=0,q2=0,q3=0,q4=0;
Mat quad1(dst, Rect(bw.cols/2,0,bw.cols/2,bw.rows/2));             //seperate 4quadrants
Mat quad2(dst, Rect(0, 0, bw.cols/2, bw.rows/2)); 
Mat quad3(dst, Rect(0, bw.rows/2,bw.cols/2, bw.rows/2)); 
Mat quad4(dst, Rect(bw.cols/2, bw.rows/2,bw.cols/2, bw.rows/2)); 
for (int i = 0; i < quad1.rows; i++)         
{
    float* ptr = quad1.ptr<float>(i);
    for (int j = 0; j < quad1.cols; j++)
      if(ptr[j]>0)                               //check for number of white pixels in quad1
		  q1++;
}
for (int i = 0; i < quad2.rows; i++)
{
    float* ptr = quad2.ptr<float>(i);
    for (int j = 0; j < quad2.cols; j++)
      if(ptr[j]>0)
		  q2++;                                  //check for number of white pixels in quad2 
}
for (int i = 0; i < quad3.rows; i++)
{
    float* ptr = quad3.ptr<float>(i);
    for (int j = 0; j < quad3.cols; j++)
      if(ptr[j]>0)                               //check for number of white pixels in quad3
		  q3++;
}
for (int i = 0; i < quad4.rows; i++)
{
    float* ptr = quad4.ptr<float>(i);
    for (int j = 0; j < quad4.cols; j++)
      if(ptr[j]>0)                               //check for number of white pixels in quad4
		  q4++;
}
//compare the number of white pixels in each quad
//if greater than set ref draw rectangle 
//the ref depends on the distance of the ball from cam
if(q1>200)                                       
rectangle(frame,Point(frame.cols,0),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q2>200)
rectangle(frame,Point(0,0),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q3>200)
rectangle(frame,Point(0,frame.rows),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q4>200)
rectangle(frame,Point(frame.cols,frame.rows),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);

	imshow("video", frame);
    imshow("bw",bw);
        if (waitKey(30) == 'q')
            break;
    }
    return 0;
}