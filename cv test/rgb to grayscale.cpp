#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("lena.jpg");

    if (img.empty())
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }

    
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);

	imshow("image", img);
	imshow("grayscale", gray);
	
    waitKey(0);

    return 0;
}