//稠密光流
#include<opencv2/opencv.hpp> 
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

void drawOpticalFlowHF(const Mat &flowdata, Mat& image, int step);//把数据画在image step步长隔几个画

int main(int argc, char**argv)
{
	VideoCapture capture;
	capture.open("E:/openCV/video/5.avi");
	
	if (!capture.isOpened())
	{
		printf("could not load video data...\n");
		return -1;

	}
	
	Mat frame,gray;
	Mat prev_frame, prev_gray;
	Mat flowResult, flowdata;
	capture.read(frame);//读取第一帧数据
	cvtColor(frame, prev_gray, COLOR_BGR2GRAY);//把前一帧全都转换成为灰度图
	namedWindow("optic flow HF", CV_WINDOW_NORMAL);
	namedWindow("input", CV_WINDOW_NORMAL);
	//从第二帧数据开始
	while (capture.read(frame))
	{
		cvtColor(frame, gray, COLOR_BGR2GRAY);//读第一帧之前先转化成灰度图像
		if (!prev_gray.empty())
		{
			calcOpticalFlowFarneback(prev_gray, gray, flowdata, 0.5, 3,15, 3, 5, 1.2, 0);
			cvtColor(prev_gray, flowResult, COLOR_GRAY2BGR);//变成彩色图像
			drawOpticalFlowHF(flowdata, flowResult,10);//每隔十个像素点进行一次操作
            imshow("optic flow HF", flowResult);//显示光流后的结果
			imshow("input", frame);//显示原来的frame
		}
		
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


void drawOpticalFlowHF(const Mat &flowdata, Mat& image, int step)
{
	for(int row = 0; row < image.rows;row++)
	{
		for (int col = 0; col < image.cols; col++)
		{
			const Point2f fxy = flowdata.at<Point2f>(row, col);
			if (fxy.x > 2 || fxy.y > 2)
			{
				line(image, Point(col, row), Point(cvRound(col + fxy.x), cvRound(row + fxy.y)), Scalar(0, 255, 0), 2, 8, 0);
				circle(image, Point(col, row), 2, Scalar(0, 0, 255), -1);//红色填充
			}
		}


	}


}


