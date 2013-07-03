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

    Mat frame,frame_gray,frame_new;
	namedWindow("video", CV_WINDOW_AUTOSIZE);
  int temp0;
    while(true)
    {
	//track bar for testing variables
		createTrackbar("trackbar1","video", &temp0,100);//createTrackbar( trackbar_type,window_name, variable,max , Threshold_Demo );	
		//createTrackbar("trackbar2","original", &temp1,1000);//createTrackbar( trackbar_type,window_name, variable,max , Threshold_Demo );	
		//createTrackbar("trackbar3","original", &temp2,30);//createTrackbar( trackbar_type,window_name, variable,max , Threshold_Demo );	
		//createTrackbar("trackbar4","original", &temp3,10);//createTrackbar( trackbar_type,window_name, variable,max , Threshold_Demo );	
		
        capture >> frame;
        if (frame.empty())
            break;
		cv::flip(frame,frame,1); // flip the image on X axis
		cvtColor( frame, frame_gray, CV_BGR2GRAY );
      
		
		//cvtColor( frame, frame_gray, CV_BGR2GRAY );// Convert it to gray
		GaussianBlur( frame_gray,frame_gray, Size(9, 9), 2, 2 );// Reduce the noise so we avoid false circle detection
		vector<Vec3f> circles;
		//HoughCircles( frame_gray, circles, CV_HOUGH_GRADIENT, 1, frame_gray.rows/8, 200, 100, 0, 0 );
		HoughCircles(frame_gray, circles, CV_HOUGH_GRADIENT, 1, frame_gray.rows/8,
                 100, 30, 40, 60 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 ); 
		 for( size_t i = 0; i < circles.size(); i++ )
			{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
			// circle outline
			circle( frame, center, radius, Scalar(0,255,0), 3, 8, 0 );
			}



		//imshow("black and white",frame_new );
		imshow("video", frame);
        imshow("transist",frame_gray);
    if (waitKey(30) == 'q')
            break;
    }

    return 0;
}