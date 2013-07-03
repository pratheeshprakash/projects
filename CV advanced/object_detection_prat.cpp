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

    Mat frame,frame_gray;
    //namedWindow("video", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        capture >> frame;
        if (frame.empty())
            break;
		cv::flip(frame,frame,1); // flip the image on X axis
		
        Mat hsv;// HSV file of the image
		cvtColor(frame, hsv, CV_BGR2HSV);	
		
		Mat bw;		
        inRange(hsv, Scalar(170,160,60), Scalar(180,256,256), bw);
		vector<vector<Point> > contours;
		findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);	
		
		Mat dst = Mat::zeros(frame.size(), frame.type());
		drawContours(dst, contours, -1, Scalar::all(255), CV_FILLED);
		dst &= frame;
		
		cvtColor( dst, dst, CV_BGR2GRAY );// Convert it to gray
		
		GaussianBlur( dst,dst, Size(9, 9), 2, 2 );// Reduce the noise so we avoid false circle detection
		vector<Vec3f> circles;
		HoughCircles( dst, circles, CV_HOUGH_GRADIENT, 1, dst.rows/8, 200, 100, 0, 100 );
		 for( size_t i = 0; i < circles.size(); i++ )
			{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
			}

		

		
		


		imshow("dst", dst);
		//imshow("black and white", bw);
		imshow("video", frame);
    
    if (waitKey(30) == 'q')
            break;
    }

    return 0;
}