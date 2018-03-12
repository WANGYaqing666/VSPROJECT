#include<opencv2/opencv.hpp>
#include<opencv2/tracking.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

int smin = 15;//s���Ͷ���Сֵ 0��43֮��ȡ����ֵ
int vmin = 40;//valueɫ����Сֵ
int vmax = 256;
int bins = 16;//binsΪ16
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

	bool firstRead = true;//��һ֡Ϊ ��
	float hrange[] = { 0, 180 };//�����H�ķ�Χ0��180
	const float* hranges = { hrange };//ָ��
	Rect selection;//����ѡ��
	Mat frame, hsv, hue, mask, hist, backprojection;//histֱ��ͼ
	Mat drawImg = Mat::zeros(300, 300, CV_8UC3);//����ֱ��ͼ��С RGB 300��300�е�������ʼ�������Ǻ�ɫ Unsigned 8bits ����1��2��3��ʾͨ������Ʃ��RGB3ͨ������CV_8UC3
	while (capture.read(frame))
	{
		if (firstRead)
		{
			Rect2d first = selectROI("CAMShift Tracking", frame);//�Ե�һ֡����ѡ�� �ֶ�ѡ��ROI ����ѡ�� openCV��չģ���е�#include<opencv2/tracking.hpp>
			//��selection��ʼ��
			selection.x = first.x;//��ʼ��
			selection.y = first.y;
			selection.width = first.width;
			selection.height = first.height;
			printf("ROI.x= %d, ROI.y= %d, width = %d, height= %d", selection.x, selection.y, selection.width, selection.height);
			
		}
		//convert to HSV
		cvtColor(frame, hsv, COLOR_BGR2HSV);//colorת��ΪHSV
		inRange(hsv, Scalar(0, smin, vmin), Scalar(180, vmax, vmax), mask);//��ȡHͨ�� ����1split������ ����������ͨ��Ҳռ�ڴ�  ����2 �Զ���ȡ
		hue = Mat(hsv.size(), hsv.depth());
		int channels[] = { 0, 0 };//����һ������
		mixChannels(&hsv, 1, &hue, 1, channels, 1);//mixChannels��Ҫ���ǰ�����ľ��󣨻�������飩��ĳЩͨ����ָ��Ƹ���Ӧ��������󣨻�������飩��ĳЩͨ���У����еĶ�Ӧ��ϵ����fromTo�����ƶ�.hsv���� 1 ���������� hue��� 1���������� ��fromTo�ĵ�һ��Ԫ�ؼ�fromTo[0]Ӧ��������������ĳ��ͨ����ǣ���fromTo�ĵڶ���Ԫ�ؼ�fromTo[1]Ӧ��������������ĳ��ͨ����ǣ����������ͻ����������fromTo[0]ͨ����������ݸ��Ƹ���������fromTo[1]ͨ����fromTo�����Ԫ��Ҳ�����������֮����һ����������ͨ����Ǻ��������Ÿ���������ͨ����ǡ�1��1�� ��hsv��hue ��hsvͼ�񻻵�hue��
		if (firstRead)
		{
			//����ֱ��ͼ�ͻ�ֱ��ͼֻ��Ҫ��һ�� ��������ǵ�һ�β�������ֱ��ͼ�ͻ�ֱ��ͼ
			// ROI ֱ��ͼ����
			Mat roi(hue, selection);//ֻ��ROI����ֱ��ͼ hue{selection}=roi
			Mat maskroi(mask, selection);//maskroi
			calcHist(&roi, 1, 0, maskroi, hist, 1, &bins, &hranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);//��һ��

			//show histogram
			int binw = drawImg.cols / bins;//16��bins ��ȡÿ��bins�Ŀ���Ƕ���
			Mat colorIndex = Mat(1, bins, CV_8UC3);//��ɫ���� ÿ��bins��ɫ��һ�� 
			for (int i = 0; i < bins; i++)//
			{
				colorIndex.at<Vec3b>(0, i) = Vec3b(saturate_cast<uchar>(i * 180 / bins), 255, 255);//���180
			}
			cvtColor(colorIndex, colorIndex, COLOR_HSV2BGR);//����BGR��ɫ�ռ�
			for (int i = 0; i < bins; i++)//��ʾֱ��ͼ �������Ƕ����drawImg��
			{
				int  val = saturate_cast<int>(hist.at<float>(i)*drawImg.rows / 255);//0��255�仯0��300 �߶�
				rectangle(drawImg, Point(i*binw, drawImg.rows), Point((i + 1)*binw, drawImg.rows - val), Scalar(colorIndex.at<Vec3b>(0, i)), -1, 8, 0);//����bins С��ֱ��ͼ��Point(i*binw, drawImg.rows)���½�, Point((i + 1)*binw���Ͻ�drawImg.rows - val�߶� Scalar(colorIndex.at<Vec3b>(0, i))��ɫ-1��䣻��Ե��������������º��������϶�����
			}
		}
		//back projection ����ӳ��
		calcBackProject(&hue, 1, 0, hist, backprojection, &hranges);
		//CAMShift tracking
		backprojection &= mask;//����� �Ѹ����ų�
		RotatedRect trackBox = CamShift(backprojection, selection, TermCriteria((TermCriteria::COUNT | TermCriteria::EPS), 10, 1));//����һֱ�ڶ�̬�仯��ѭ�����ٵ���������TermCriteria::COUNT�涨��ѭ�����ﵽ TermCriteria::EPS֮ǰ���ĵ����һ֡���ĵ��ֵ<1 10��ѭ�����򷵻���ת����

		//draw location on frame
		ellipse(frame, trackBox, Scalar(0, 0, 255), 3, 8);
		if (firstRead)
		{
			firstRead = false;
		}
		imshow("CAMShift Tracking",frame);
		imshow("ROI Histogram", drawImg);//����ROIֱ��ͼ
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