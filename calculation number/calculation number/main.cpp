
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char**) {
	VideoCapture capture;
	capture.open("D:/vcprojects/images/768x576.avi");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	namedWindow("input video", CV_WINDOW_AUTOSIZE);
	namedWindow("motion objects", CV_WINDOW_AUTOSIZE);

	// 初始BS模型
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();//模型
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));//形态学操作 获取结构元素 小于干扰的去掉

	vector<vector<Point>> contours;//？
	vector<Vec4i> hireachy;//？
	int count = 0;//计数

	Mat frame, gray, mogMask;
	while (capture.read(frame)) {
		imshow("input video", frame);
		pMOG2->apply(frame, mogMask);//使用模型 默认-1自动计算 1每一帧都重新计算 0不更新 若认为他自己计算没有你指定的好给0到1之间的一个值
		threshold(mogMask, mogMask, 100, 255, THRESH_BINARY);//threshold 方法是通过遍历灰度图中点，将图像信息二值化，处理过后的图片只有二种色值。
		morphologyEx(mogMask, mogMask, MORPH_OPEN, kernel, Point(-1, -1));//先腐蚀后膨胀 形态学开操作
																		  //提取ROI
		findContours(mogMask, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));//查找轮廓 获取最外层的轮廓
		count = 0;
		char numText[8];
		for (size_t t = 0; t < contours.size(); t++)
		{
			double area = contourArea(contours[t]);
			if (area < 1000) continue;//面积小于1000基本是30X30像素 小于1000的除去
			Rect selection = boundingRect(contours[t]);//知道那个区域有多大
			if (selection.width < 30 || selection.height < 30) continue;//车肯定不是一条直线 所选区域长小于30或者高小于30则不是车
			count++;//车多了一辆
			rectangle(frame, selection, Scalar(0, 0, 255), 2, 8);//把车画成矩形 8是8厘米粗的线

			sprintf(numText, "%d", count);//sprintf和printf差不多
			putText(frame, numText, Point(selection.x, selection.y), CV_FONT_NORMAL, FONT_HERSHEY_PLAIN, Scalar(255, 0, 0), 1, 8);//文字显示出来 thickness1 后面默认
		}

		imshow("motion objects", frame);
		char c = waitKey(50);
		if (c == 27) // ESC
		{
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}












