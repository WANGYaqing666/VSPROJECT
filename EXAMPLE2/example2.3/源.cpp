#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

int main()
{
	namedWindow("Example3", WINDOW_AUTOSIZE);
	VideoCapture cap;
	cap.open("tree.avi");

	Mat frame;
	for (;;) //��������ѭ����䣬ֱ��ִ����ָ�����Ƴ�Ϊֹ
	{
		cap >> frame;
		if (frame.empty()) break;
		imshow("Example3", frame);
		if (cv::waitKey(33) >= 0)break;

	}
	return 0;

}