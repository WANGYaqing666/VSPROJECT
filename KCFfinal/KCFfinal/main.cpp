
/*
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) 
{
	VideoCapture capture;
	capture.open("E:/openCV/video/2.mp4");
	if (!capture.isOpened()) 
	{
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	namedWindow("tracker demo", CV_WINDOW_NORMAL);
	//Ptr<Tracker> tracker = Tracker::create("KCF");//
	//Ptr<Tracker> getTrackerByIndex(int index);
	Ptr<Tracker> tracker;
	tracker = TrackerKCF::create();
		
	capture.read(frame);
	Rect2d  roi = selectROI("tracker demo", frame);
	if (roi.width == 0 || roi.height == 0) //没选择
	{
		return -1;
	}
	tracker->init(frame, roi);//通过ROI初始化tracker
	while (capture.read(frame)) 
	{
		tracker->update(frame, roi);//更新ROI
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 8, 0);//蓝色
		imshow("tracker demo", frame);

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
*/

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("E:/openCV/video/1.AVI");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	namedWindow("KCF tracker", CV_WINDOW_NORMAL);
	
	Ptr<Tracker> tracker = Tracker::create("KCF");
	
	capture.read(frame);
	Rect2d  roi = selectROI("KCF tracker", frame);
	
	if (roi.width == 0 || roi.height == 0) {
		return -1;
	}
	tracker->init(frame, roi);
	while (capture.read(frame)) 
	{
		tracker->update(frame, roi);
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 8, 0);
		imshow("KCF tracker", frame);

		char c = waitKey(1);
		if (c == 27) {
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}








/*
// Standard include files
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <iostream>


using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	VideoCapture capture;
	// Set up tracker.
	// Instead of MIL, you can also use
	// BOOSTING, KCF, TLD, MEDIANFLOW or GOTURN
	Ptr<Tracker> tracker;
	tracker = TrackerKCF::create();
	//Ptr<Tracker> tracker = Tracker::create("MEDIANFLOW");

	// Read video
	VideoCapture video("E:/openCV/video/balltest.mp4");

	double dWidth = video.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = video.get(CV_CAP_PROP_FRAME_HEIGHT);

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	VideoWriter oVideoWriter("/Users/Stephanie/Desktop/Videotest.avi", CV_FOURCC('P', 'I', 'M', '1'), 20, frameSize, true);

	// Check video is open
	if (!video.isOpened())
	{
		cout << "Could not read video file" << endl;
		return 1;
	}

	// Read first frame.
	Mat frame;
	video.read(frame);

	// Define an initial bounding box
	Rect2d bbox(123, 87, 132, 176);

	// Uncomment the line below if you
	// want to choose the bounding box
	capture.read(frame);
	Rect2d  roi = selectROI("tracker demo", frame);
	// bbox = selectROI(frame, false);

	// Initialize tracker with first frame and bounding box
	tracker->init(frame, bbox);

	while (video.read(frame))
	{
		// Update tracking results
		tracker->update(frame, bbox);
		// Draw bounding box
		rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

		// Display result
		imshow("video", frame);

		oVideoWriter.write(frame);

		//int k = waitKey(1);
		//if(k == 27) break;
		if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
*/
