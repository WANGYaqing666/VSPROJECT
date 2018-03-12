
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

	// ��ʼBSģ��
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();//ģ��
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));//��̬ѧ���� ��ȡ�ṹԪ�� С�ڸ��ŵ�ȥ��

	vector<vector<Point>> contours;//��
	vector<Vec4i> hireachy;//��
	int count = 0;//����

	Mat frame, gray, mogMask;
	while (capture.read(frame)) {
		imshow("input video", frame);
		pMOG2->apply(frame, mogMask);//ʹ��ģ�� Ĭ��-1�Զ����� 1ÿһ֡�����¼��� 0������ ����Ϊ���Լ�����û����ָ���ĺø�0��1֮���һ��ֵ
		threshold(mogMask, mogMask, 100, 255, THRESH_BINARY);//threshold ������ͨ�������Ҷ�ͼ�е㣬��ͼ����Ϣ��ֵ������������ͼƬֻ�ж���ɫֵ��
		morphologyEx(mogMask, mogMask, MORPH_OPEN, kernel, Point(-1, -1));//�ȸ�ʴ������ ��̬ѧ������
																		  //��ȡROI
		findContours(mogMask, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));//�������� ��ȡ����������
		count = 0;
		char numText[8];
		for (size_t t = 0; t < contours.size(); t++)
		{
			double area = contourArea(contours[t]);
			if (area < 1000) continue;//���С��1000������30X30���� С��1000�ĳ�ȥ
			Rect selection = boundingRect(contours[t]);//֪���Ǹ������ж��
			if (selection.width < 30 || selection.height < 30) continue;//���϶�����һ��ֱ�� ��ѡ����С��30���߸�С��30���ǳ�
			count++;//������һ��
			rectangle(frame, selection, Scalar(0, 0, 255), 2, 8);//�ѳ����ɾ��� 8��8���״ֵ���

			sprintf(numText, "%d", count);//sprintf��printf���
			putText(frame, numText, Point(selection.x, selection.y), CV_FONT_NORMAL, FONT_HERSHEY_PLAIN, Scalar(255, 0, 0), 1, 8);//������ʾ���� thickness1 ����Ĭ��
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












