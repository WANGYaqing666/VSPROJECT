#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) 
{
	VideoCapture capture;
	capture.open("E:/openCV/video/1.AVI");
	if (!capture.isOpened()) 
	{
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	
	namedWindow("MEDIANFLOW", CV_WINDOW_NORMAL);
	
	Ptr<Tracker> tracker = Tracker::create("MEDIANFLOW");
	capture.read(frame);
	
	Rect2d  roi = selectROI("MEDIANFLOW", frame);
	if (roi.width == 0 || roi.height == 0) 
	{
		return -1;
	}
	tracker->init(frame, roi);
	while (capture.read(frame))
	{
		tracker->update(frame, roi);
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 8, 0);
		
		imshow("MEDIANFLOW", frame);


		char c = waitKey(1);
		if (c == 27) 
		{
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}
