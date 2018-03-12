#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("E:/openCV/video/1.AVI");
	if (!capture.isOpened()) 
	{
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	vector<KeyPoint> keypoints;
	namedWindow("MIL", CV_WINDOW_NORMAL);

	Ptr<Tracker> tracker = Tracker::create("MIL");

	capture.read(frame);
	Rect2d  roi = selectROI("MIL", frame);

	if (roi.width == 0 || roi.height == 0) 
	{
		return -1;
	}
	tracker->init(frame, roi);
	//Point p(roi.width/2, roi.height/2);
	while (capture.read(frame))
	{
		tracker->update(frame, roi);
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 8, 0);
		
		//circle(frame, p, 8, Scalar(0, 255, 0)); 

		imshow("MIL", frame);

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