#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <vector>
#include <stdio.h>
#include <opencv/cv.h>

using namespace cv;
using namespace std;
void drawLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = 8;
  line( img,start,end,Scalar( 255, 255, 255),thickness,lineType ); //line( img,start,end,Scalar( 0, 0, 0 ),thickness,lineType );
}
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
	
	 
		
	Mat frame,frame_temp;
	Point curCir=Point(0,0), lastCir=Point(0,0);//position of current and last circle
	
	int p=10,quit=0;
	char text[200];
	
	while(true)
    {
        capture >> frame;
        if (frame.empty())
            break;
		if (frame_temp.empty())
			{
		     frame_temp.create(frame.rows,frame.cols,CV_8UC3);// always use .create function to allocate size
			}
		cv::flip(frame,frame,1); // flip the image on X axis
		
		Mat bw;
		cvtColor( frame, bw, CV_BGR2GRAY );
			
		GaussianBlur( bw,bw, Size(9, 9), 2, 2 );// Reduce the noise so we avoid false circle detection
		vector<Vec3f> circles;
		
		
		HoughCircles(bw, circles, CV_HOUGH_GRADIENT, 1, bw.rows/8,
                 30, 52, 60, 100 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 ); //http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=houghcircles#cv2.HoughCircles
				 
		 for( size_t i = 0; i < circles.size(); i++ )
			{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
			
			lastCir=curCir;// store centre values of circles
			curCir=center;
	
			// circle outline
			circle( frame, center, radius, Scalar(0,255,0), 3, 8, 0 );
			}
		
		drawLine( frame_temp,lastCir,curCir );	//draw line from last position to current //drawLine( mat img,start,end);
		
		//temporary variable
		sprintf(text, "p=%d", (int)p);	// convert variables to text so that they can be displayed on screen
	    putText(frame, text, Point(10,20),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0,0), 1, CV_AA);	// display text on screen

		sprintf(text, "x=%d y=%d", (int)curCir.x,(int)curCir.y);	// convert variables to text so that they can be displayed on screen
	    putText(frame, text, Point(curCir.x-60,curCir.y-60),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0,0), 1, CV_AA);	// display text on screen

		//frame|=frame_temp;// overlay the drawing on the image, another way would be//
		add(frame, frame_temp, frame);
		
		imshow("original",frame);
		imshow("Black and White",bw);
				
		switch(waitKey(30))
		{
		case 'q': quit=1;
				break;
        case 'm': p++;
				break;
		case 'n': p--;
				if(p<=0)
				p=1;
				break;
		}
		if(quit==1)
		break;
	}	
}