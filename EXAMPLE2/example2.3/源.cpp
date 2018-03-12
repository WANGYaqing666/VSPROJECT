#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

int main()
{
	namedWindow("Example3", WINDOW_AUTOSIZE);
	VideoCapture cap;
	cap.open("tree.avi");

	Mat frame;
	for (;;) //无条件的循环语句，直到执行你指定的推出为止
	{
		cap >> frame;
		if (frame.empty()) break;
		imshow("Example3", frame);
		if (cv::waitKey(33) >= 0)break;

	}
	return 0;

}