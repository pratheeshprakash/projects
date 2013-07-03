#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
   VideoCapture capture;

    if (argc == 2)
        capture.open(argv[1]);  // Open file
    else
        capture.open(1);        // Open camera device 0 for default camera and 1 for external camera connected if any 

    if (!capture.isOpened())
		{
        cout << "Cannot open video device or file!" << endl;
        return -1;
		}
	while(true)
    {
    capture >> src;
    if (src.empty())
            break;
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    Mat bw;
    inRange(hsv, Scalar(19, 204, 153), Scalar(27, 255, 255), bw);

    vector<vector<Point> > contours;
    findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    Mat dst = Mat::zeros(src.size(), src.type());
    drawContours(dst, contours, -1, Scalar(0,255,255), CV_FILLED);// (Image to store, cntours variable, -1 represents draw all, colour, filled contour)q

    //dst &= src;

    imshow("src", src);
	imshow("hsv1",bw);
    imshow("dst", dst);
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
    return 0;
}