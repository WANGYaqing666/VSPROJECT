#include<opencv2/opencv.hpp>
#include<opencv2/tracking.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

int smin = 15;//s饱和度最小值 0到43之间取个均值
int vmin = 40;//value色调最小值
int vmax = 256;
int bins = 16;//bins为16
int main(int argc, char** argv)
{
	VideoCapture capture;
	capture.open("E:/openCV/video/1.AVI");
	if (!capture.isOpened())
	{
		printf("could not find video data file...\n");
		return -1;
	}
	namedWindow("CAMShift Tracking", CV_WINDOW_NORMAL);
	namedWindow("ROI Histogram", CV_WINDOW_NORMAL);

	bool firstRead = true;//第一帧为 是
	float hrange[] = { 0, 180 };//表格中H的范围0到180
	const float* hranges = { hrange };//指针
	Rect selection;//窗口选择
	Mat frame, hsv, hue, mask, hist, backprojection;//hist直方图
	Mat drawImg = Mat::zeros(300, 300, CV_8UC3);//定义直方图大小 RGB 300行300列的零矩阵初始化背景是黑色 Unsigned 8bits 最后的1、2、3表示通道数，譬如RGB3通道就用CV_8UC3
	while (capture.read(frame))
	{
		if (firstRead)
		{
			Rect2d first = selectROI("CAMShift Tracking", frame);//对第一帧进行选择 手动选择ROI 中心选择 openCV扩展模块中的#include<opencv2/tracking.hpp>
			//对selection初始化
			selection.x = first.x;//初始化
			selection.y = first.y;
			selection.width = first.width;
			selection.height = first.height;
			printf("ROI.x= %d, ROI.y= %d, width = %d, height= %d", selection.x, selection.y, selection.width, selection.height);
			
		}
		//convert to HSV
		cvtColor(frame, hsv, COLOR_BGR2HSV);//color转换为HSV
		inRange(hsv, Scalar(0, smin, vmin), Scalar(180, vmax, vmax), mask);//获取H通道 方法1split劈出来 但另外两个通道也占内存  方法2 自动获取
		hue = Mat(hsv.size(), hsv.depth());
		int channels[] = { 0, 0 };//定义一个数组
		mixChannels(&hsv, 1, &hue, 1, channels, 1);//mixChannels主要就是把输入的矩阵（或矩阵数组）的某些通道拆分复制给对应的输出矩阵（或矩阵数组）的某些通道中，其中的对应关系就由fromTo参数制定.hsv输入 1 输入矩阵个数 hue输出 1输出矩阵个数 组fromTo的第一个元素即fromTo[0]应该填入输入矩阵的某个通道标记，而fromTo的第二个元素即fromTo[1]应该填入输出矩阵的某个通道标记，这样函数就会把输入矩阵的fromTo[0]通道里面的数据复制给输出矩阵的fromTo[1]通道。fromTo后面的元素也是这个道理，总之就是一个输入矩阵的通道标记后面必须跟着个输出矩阵的通道标记。1有1对 从hsv到hue 把hsv图像换到hue中
		if (firstRead)
		{
			//计算直方图和画直方图只需要做一次 所以如果是第一次才做计算直方图和画直方图
			// ROI 直方图计算
			Mat roi(hue, selection);//只对ROI计算直方图 hue{selection}=roi
			Mat maskroi(mask, selection);//maskroi
			calcHist(&roi, 1, 0, maskroi, hist, 1, &bins, &hranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);//归一化

			//show histogram
			int binw = drawImg.cols / bins;//16个bins 获取每个bins的宽度是多少
			Mat colorIndex = Mat(1, bins, CV_8UC3);//颜色索引 每个bins颜色不一样 
			for (int i = 0; i < bins; i++)//
			{
				colorIndex.at<Vec3b>(0, i) = Vec3b(saturate_cast<uchar>(i * 180 / bins), 255, 255);//最大180
			}
			cvtColor(colorIndex, colorIndex, COLOR_HSV2BGR);//换成BGR颜色空间
			for (int i = 0; i < bins; i++)//显示直方图 画在我们定义的drawImg上
			{
				int  val = saturate_cast<int>(hist.at<float>(i)*drawImg.rows / 255);//0到255变化0到300 高度
				rectangle(drawImg, Point(i*binw, drawImg.rows), Point((i + 1)*binw, drawImg.rows - val), Scalar(colorIndex.at<Vec3b>(0, i)), -1, 8, 0);//画出bins 小的直方图；Point(i*binw, drawImg.rows)左下角, Point((i + 1)*binw右上角drawImg.rows - val高度 Scalar(colorIndex.at<Vec3b>(0, i))颜色-1填充；相对的两个点左上右下和左下右上都可以
			}
		}
		//back projection 反向映射
		calcBackProject(&hue, 1, 0, hist, backprojection, &hranges);
		//CAMShift tracking
		backprojection &= mask;//与操作 把干扰排除
		RotatedRect trackBox = CamShift(backprojection, selection, TermCriteria((TermCriteria::COUNT | TermCriteria::EPS), 10, 1));//窗口一直在动态变化；循环跟踪的两个条件TermCriteria::COUNT规定的循环数达到 TermCriteria::EPS之前中心点和下一帧中心点差值<1 10个循环，则返回旋转矩形

		//draw location on frame
		ellipse(frame, trackBox, Scalar(0, 0, 255), 3, 8);
		if (firstRead)
		{
			firstRead = false;
		}
		imshow("CAMShift Tracking",frame);
		imshow("ROI Histogram", drawImg);//画出ROI直方图
		char c = waitKey(1);// ESC
		if (c == 27)
		{
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}