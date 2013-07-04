#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <vector>
#include <stdio.h>
#include <opencv/cv.h>
#include "serialport.h"

using namespace cv;
using namespace std;

int pan=90,tilt=90,min_pan=1,max_pan=180,min_tilt=40,max_tilt=130; //keep min_pan and min_tilt above 0 otherwise error will occur
char sendbuffer[10],readbuffer[99];

HANDLE h = openSerialPort("COM4",B9600,one,off);					//initiallise the comport cnage to your arduinos comport

void movemotor()
{
// check if pan and tilt values lie between stipulated limits
	if(pan<min_pan) 					
		pan=min_pan;
	else if(pan>max_pan) 									// check if values lie between spulated limits
		pan=max_pan;	
	if(tilt<min_tilt)
		tilt=min_tilt;
	else if(tilt>max_tilt)
		tilt=max_tilt;
		
	sprintf(sendbuffer, "<P%cT%c>",pan,tilt);				// convert variables to character( transmission protocol "<Ppan_valueTtilt_value" 
	writeToSerialPort(h,sendbuffer,strlen(sendbuffer));		// send serial command to motors
	//readFromSerialPort(h,readbuffer,99); 					// Uncomment if arduino send Acknowledgment signals (for future use) 
}
void drawLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = 8;
  line( img,start,end,Scalar( 255, 255, 255),thickness,lineType ); //line( img,start,end,Scalar( 0, 0, 0 ),thickness,lineType );
}

int main(int argc, char** argv)
{
	
	VideoCapture capture;
	capture.open(1);        										// Open camera device 0 for default camera and 1 for external camera connected if any 
    
    if (!capture.isOpened())
		{
        cout << "Cannot open video device or file!" << endl;
        return -1;
		}
			
	Mat frame;
	// temporary variables only for debugging
	int p=3,temp0=90;
	// variables used in program
	int quit=0;
	char text[50];
	int l=(frame.cols/4),b=(frame.rows/4);
	
	
  while(true)
    {
	capture >> frame;
    if (frame.empty())
        break;
	
	//cvCreateTrackbar( "trackbar1", "original", &temp0, 255, NULL);
	
	
	cv::flip(frame,frame,1); 	// flip the image on X axis
								//Convert image to hsv
	Mat hsv;					// HSV file of the image
	
	//filter pixels with RED and store in BW binary image
	cvtColor(frame, hsv, CV_BGR2HSV);
	Mat bw,bw1;
	inRange(hsv, Scalar(165,160,60), Scalar(180,256,256), bw);	//red	first part // red is cyclic around 0-15 and 165 - 180 as suggested by Utkarsh(liquidmetal)
	inRange(hsv, Scalar(0,160,60), Scalar(15,256,256), bw1);	//red second part 
	bw|=bw1;													//combine both images
	
	GaussianBlur(bw, bw, Size(11,11), 0);						// blur image to mix all the small blobs
	
	//look for blobs
	vector< vector<Point> > contours;
	findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	bw = Mat::zeros(bw.size(), bw.type());						// reinitialise bw to store mask image
	
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
		minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		}
		
	for( int i = 0; i< contours.size(); i++ )
		{
		circle( frame, center[i], (int)radius[i], Scalar(255,0,0), 1, 8, 0);//Circle(img, center, radius, color, thickness=1, lineType=8, shift=0) thickness in -ve will give filled circle
		cen=center[i];
		rad=radius[i];
		}
	int l=(frame.cols/6),b=(frame.rows/6); // decides the size of the rectangle in which camera will not move	
	Point pt1 = Point((frame.cols/2)-l,(frame.rows/2)-b),pt2 = Point((frame.cols/2)+l,(frame.rows/2)+b);	
   	rectangle(frame, pt1,pt2, Scalar(0,255,255), 1, 8, 0 );
	
	//move the motors 
	
	int speed=3;
	if(cen.x<pt1.x)
	pan+=speed;
	else if(cen.x>pt2.x)
	pan-=speed;
	if(cen.y<pt1.y)
	tilt+=speed;
	else if(cen.y>pt2.y)
	tilt-=speed;
	
	if(cen.x==0 && cen.y==0)		//centre servos if object not found
	{
		pan=90;
		tilt=90;
	}
	
	movemotor(); 				// send values to motor
	
	//put text on screen
	//sprintf(text, "%dby%d",(int) pt1.x,(int)pt1.y);	// convert variables to text so that they can be displayed on screen
	//putText(frame, text, Point(100,20),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0,0), 1, CV_AA);	// display text on screen
	//char text1[10];
	//sprintf(text1, "centre=%d,%d",(int)cen.x,(int)cen.y);	// convert variables to text so that they can be displayed on screen
	//putText(frame, sendbuffer, Point(100,50),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0,0), 1, CV_AA);	// display text on screen
	
	imshow("original",frame);
		
		
	switch(waitKey(30))						//routine used for quiting and incrementing variaables if wanted
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
closeSerialPort(h);	
}
