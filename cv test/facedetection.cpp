#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
    CascadeClassifier cascade;

    if (!cascade.load("haarcascade_frontalface_alt.xml"))
        return -1;

    Mat src = imread("people.jpg");
    if (src.empty())
        return -1;

    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    equalizeHist(gray, gray);

    vector<Rect> faces;
    cascade.detectMultiScale(gray, faces, 1.2, 3);

    for (int i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        rectangle(src, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), CV_RGB(0,255,0));
    }

    imshow("src", src);
    waitKey(0);

    return 0;
}