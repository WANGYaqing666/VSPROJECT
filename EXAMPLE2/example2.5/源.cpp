#include<opencv2/opencv.hpp>
using namespace cv;

int main()
{
	Mat image = imread("1.tif");
	//Create some windows to show the input and outout images in.
	namedWindow("Example2_5-in", WINDOW_AUTOSIZE);
	namedWindow("Example2_5-in", WINDOW_AUTOSIZE);
	//create a window to show our input image
	imshow("Example2_5-in", image);
	//create an image to hold the smoothed output

	Mat out;
	//Do the smoothing
	GaussianBlur(image, out, Size(5, 5), 3, 3);
	GaussianBlur(out, out, Size(5, 5), 3, 3);
	//Show the smoothed image in the output window

	imshow("Example2_5-out", out);
	waitKey(0);

}