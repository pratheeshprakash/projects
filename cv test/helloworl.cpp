#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);

    if (img.empty())
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }

    namedWindow("image", CV_WINDOW_AUTOSIZE);
    imshow("image", img);
    waitKey(0);

    return 0;
}