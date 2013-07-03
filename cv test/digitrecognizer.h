
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

#define MAX_NUM_IMAGES	60000

class DigitRecognizer
{
public:
	DigitRecognizer();
	~DigitRecognizer();
	
	bool train(char* trainPath, char* labelsPath);
	int classify(Mat img);

private:
	cv::Mat preprocessImage(Mat img);
	int readFlippedInteger(FILE *fp);

private:
	KNearest	*knn;
	int numRows, numCols, numImages;
};