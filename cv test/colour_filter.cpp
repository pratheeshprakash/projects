#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("colourb.jpeg");
    if (src.empty())
        return -1;

    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    Mat bw;
    inRange(hsv, Scalar(19, 204, 153), Scalar(27, 255, 255), bw);

    vector<vector<Point> > contours;
    findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    Mat dst = Mat::zeros(src.size(), src.type());
    drawContours(dst, contours, -1, Scalar(0,255,255), CV_FILLED);// (Image to store, cntours variable, -1 represents draw all, colour, filled contour)

    dst &= src;

    imshow("src", src);
	imshow("hsv1",bw);
    imshow("dst", dst);
    waitKey(0);

    return 0;
}