#include <opencv2/highgui/highgui.hpp>
#include <iostream>

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

    Mat frame;
    namedWindow("video", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        capture >> frame;
        if (frame.empty())
            break;
        imshow("video", frame);
        if (waitKey(30) == 'q')
            break;
    }

    return 0;
}