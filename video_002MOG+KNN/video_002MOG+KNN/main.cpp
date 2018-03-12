#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char**) 
{
	VideoCapture capture;
	capture.open("E:/openCV/video/2.mp4");
	if (!capture.isOpened()) 
	{
		printf("could not find the video file...\n");
		return -1;
	}
	// create windows
	Mat frame;
	Mat bsmaskMOG2, bsmaskKNN;
	namedWindow("input video", CV_WINDOW_NORMAL);
	namedWindow("MOG2", CV_WINDOW_NORMAL);
	namedWindow("KNN Model", CV_WINDOW_NORMAL);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

	// intialization BS
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();
	Ptr<BackgroundSubtractor> pKNN = createBackgroundSubtractorKNN();
	while (capture.read(frame)) 
	{
		imshow("input video", frame);

		// BSM
		pMOG2->apply(frame, bsmaskMOG2);
		pKNN->apply(frame, bsmaskKNN);
		//morphologyEx(bsmaskMOG2, bsmaskMOG2, MORPH_OPEN, kernel, Point(-1, -1));
		

		// show mask
		imshow("MOG2", bsmaskMOG2);
		imshow("KNN Model", bsmaskKNN);
		char c = waitKey(100);
		if (c == 27) {
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}