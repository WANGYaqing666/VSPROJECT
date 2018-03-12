#include<opencv2/opencv.hpp>
using namespace cv;
int main()
{
	Mat img1, img2;
	namedWindow("Example2_5-in", WINDOW_AUTOSIZE);
	namedWindow("Example2_5-in", WINDOW_AUTOSIZE);
	img1 = imread("1.tif");
	imshow("Example1", img1);
	pyrDown(img1, img2);
	imshow("Example2", img2);
	waitKey(0);
	return 0;
}