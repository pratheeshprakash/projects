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
        capture.open(1);        // Open camera device

    if (!capture.isOpened())
    {
        cout << "Cannot open video device or file!" << endl;
        return -1;
    }

    Mat frame,frame_gray;
    namedWindow("video", CV_WINDOW_AUTOSIZE);

    while(true)
    {
	   
		  
        capture >> frame;
        if (frame.empty())
            break;
		cv::flip(frame,frame,1); // flip the image on X axis
		
        Mat hsv;// HSV file of the image
		cvtColor(frame, hsv, CV_BGR2HSV);	
		
		Mat bw;		
        inRange(hsv, Scalar(170,160,60), Scalar(180,256,256), bw);//red
		//inRange(hsv, Scalar(19, 204, 153), Scalar(27, 255, 255), bw);//yellow

		vector<vector<Point> > contours;
		findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);	
		
		//cvtColor( frame, frame_gray, CV_BGR2GRAY );// Convert it to gray
		GaussianBlur( bw,bw, Size(9, 9), 2, 2 );// Reduce the noise so we avoid false circle detection
		vector<Vec3f> circles;
		HoughCircles( bw, circles, CV_HOUGH_GRADIENT, 1, bw.rows/8, 200, 100, 0, 50 );
		 for( size_t i = 0; i < circles.size(); i++ )
			{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
			}

		Mat dst = Mat::zeros(frame.size(), frame.type());
		drawContours(dst, contours, -1, Scalar::all(255), CV_FILLED);

		dst &= frame;
		
int q1=0,q2=0,q3=0,q4=0;
Mat quad1(dst, Rect(frame.cols/2,0,frame.cols/2,frame.rows/2));             //seperate 4quadrants
Mat quad2(dst, Rect(0, 0, frame.cols/2, frame.rows/2)); 
Mat quad3(dst, Rect(0, frame.rows/2,frame.cols/2, frame.rows/2)); 
Mat quad4(dst, Rect(frame.cols/2, frame.rows/2,frame.cols/2, frame.rows/2)); 
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
int size_of_object =900;
//compare the number of white pixels in each quad
//if greater than set ref draw rectangle 
//the ref depends on the distance of the ball from cam
if(q1>size_of_object)                                       
rectangle(frame,Point(frame.cols,0),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q2>size_of_object)
rectangle(frame,Point(0,0),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q3>size_of_object)
rectangle(frame,Point(0,frame.rows),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);
if(q4>size_of_object)
rectangle(frame,Point(frame.cols,frame.rows),Point(frame.cols/2,frame.rows/2),CV_RGB(255,0,0),3,8,0);


		imshow("dst", dst);
		imshow("black and white", bw);
		imshow("video", frame);
    
    if (waitKey(30) == 'q')
            break;
    }

    return 0;
}