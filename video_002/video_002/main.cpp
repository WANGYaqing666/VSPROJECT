//混合高斯
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char**) 
{
	VideoCapture capture;
	capture.open("E:/openCV/video/video_004.avi");
	if (!capture.isOpened()) 
	{
		printf("could not find the video file...\n");
		return -1;
	}
	// create windows
	Mat frame;
	Mat bsmaskMOG2;//bsmaskKNN;
	namedWindow("input video", CV_WINDOW_AUTOSIZE);
	namedWindow("MOG2", CV_WINDOW_AUTOSIZE);//混合高斯
	//namedWindow("KNN Model", CV_WINDOW_AUTOSIZE);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

	// intialization BS
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();//定义指针，带有默认参数
	//Ptr<BackgroundSubtractor> pKNN = createBackgroundSubtractorKNN();
	while (capture.read(frame)) 
	{
		imshow("input video", frame);

		// MOG BS
		pMOG2->apply(frame, bsmaskMOG2);
		//morphologyEx(bsmaskMOG2, bsmaskMOG2, MORPH_OPEN, kernel, Point(-1, -1));
		imshow("MOG2", bsmaskMOG2);

		// KNN BS mask
		//pKNN->apply(frame, bsmaskKNN);
		//imshow("KNN Model", bsmaskKNN);
		char c = waitKey(100);//中途可以关掉 等待100毫秒
		if (c == 27) 
		{
			break;
		}
	}

	capture.release();//视频一定要release
	waitKey(0);
	return 0;
}