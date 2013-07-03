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
	capture.open(1);        // Open camera device 0 for default camera and 1 for external camera connected if any 
    Point lastCir,curCir;
    if (!capture.isOpened())
		{
        cout << "Cannot open video device or file!" << endl;
        return -1;
		}
			
	Mat frame,frame_temp;
	// temporary variables only for debugging
	int p=0,temp0=1,temp1=1,temp2=1,temp3=0;
	int quit=0;
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
			
	//Convert image to hsv
	Mat hsv;// HSV file of the image
	cvtColor(frame, hsv, CV_BGR2HSV);
	//filter pixels with RED and store in BW binary image
	Mat bw;
	inRange(hsv, Scalar(170,160,60), Scalar(180,256,256), bw);//red	
	//look for blobs
	vector< vector<Point> > contours;
	findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	bw = Mat::zeros(bw.size(), bw.type());// reinitialise bw to store mask image
	
	/*
	Before drawing all contours you could also decide
	to only draw the contour of the largest connected component
	found. 
	*/
	vector<double> areas(contours.size());
	for(int i = 0; i < contours.size(); i++)
       areas[i] = contourArea(Mat(contours[i]));
	
	//find position of biggest blob and draw it 
    double max;
    Point maxPosition;
    minMaxLoc(Mat(areas),0,&max,0,&maxPosition);
    
	//new binary image with mask on the biggest blob
	drawContours(bw, contours, maxPosition.y, Scalar(255), CV_FILLED);
	findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		
	//make a rectangular mask around the biggest blob and find the radius of the biggest blob
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	Point2f cen;
	float rad= 0;

	for( int i = 0; i < contours.size(); i++ )
		{   
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		}
		
	Mat mask = Mat::zeros(frame.size(), frame.type());//new image to mask with the frame, both have to be of same type other wise we cannot do and operation	
	
	for( int i = 0; i< contours.size(); i++ )
		{
		circle( mask, center[i], (int)radius[i]+10, Scalar(255,255,255), -1, 8, 0);//Circle(img, center, radius, color, thickness=1, lineType=8, shift=0) thickness in -ve will give filled circle
		//rectangle( mask, boundRect[i].tl(), boundRect[i].br(), Scalar(255,255,255), -1, 8, 0 );
		rad=radius[i];
		}
		
    mask&=frame; // extract the region that has red and save in mask
	
	//circle detection
    cvtColor(mask, bw, CV_BGR2GRAY );// convert the isolated region to black and white
	GaussianBlur( bw,bw, Size(9, 9), 2, 2 );// Reduce the noise so we avoid false circle detection
	vector<Vec3f> circles;
	//if(rad>=10)//filter out blobs smaller than 100 pixels
	//{
	HoughCircles(bw, circles, CV_HOUGH_GRADIENT, 1, bw.rows/8,
    100, 30, 0, rad+8// change the last two parameters
                           // (min_radius & max_radius) to detect larger circles
                 ); //http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=houghcircles#cv2.HoughCircles
	//HoughCircles( bw, circles, CV_HOUGH_GRADIENT, 1, bw.rows/8, 200, 100, 0, 50 );
	
	for( size_t i = 0; i < circles.size(); i++ )
		{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
			
		lastCir=curCir;// store centre values of circles//debug
		curCir=center;//debug
	
		// circle outline
		circle( frame, center, radius, Scalar(0,255,0), 3, 8, 0 );
		}
	if(lastCir.x!=0)// stop from drawing line from (0,0)
	drawLine( frame_temp,lastCir,curCir );	//draw line from last position to current //drawLine( mat img,start,end);	
	//}
	
	frame|=frame_temp;//superimpose lines on the original 
	
	//put text on screen
	sprintf(text, "use any Red object to draw on screen,Press Q to Quit");	// convert variables to text so that they can be displayed on screen
	putText(frame, text, Point(20,20),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0,0), 1, CV_AA);	// display text on screen
	
	imshow("original",frame);
		
		
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