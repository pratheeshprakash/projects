#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("lena.png");

    if (img.empty())
    {
        cout << "Cannot open image!" << endl;
        return -1;
    }

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    imshow("image", img);
    imshow("gray", gray);
    waitKey(0);

    return 0;
}