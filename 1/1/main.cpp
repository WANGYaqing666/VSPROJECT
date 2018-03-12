#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int MAX_POINT_COUNT = 500;

bool needToInit();

void main()
{
	char *fn = "E:\\VSPROJECT\\1\\1\\1.mp4";
	VideoCapture cap;
	Mat source, result, gray, lastGray; //gray, lastGray��Ӧ��һ֡�ͱ�֡�Ҷ�ͼ
	vector<Point2f> points[2],temp; // ��Ӧ��һ֡�ͱ�֡�������㣬��һ֡�Ǹ����ģ���֡ʱԤ����
	vector<uchar> status; // ÿһ��������״̬
	vector<float> err; // ÿһ������������


	cap.open(fn);
	if (!cap.isOpened())
	{
		cout << "�޷�����ƵԴ�������ļ�������ͷ" << endl;
		return;
	}

	for (;;)
	{
		cap >> source;
		if (source.empty())
			break;

		cvtColor(source, gray, COLOR_BGR2GRAY);
		// �����Ǵ���
		if ( points[0].size()<10 )// ����̫�٣����¼��������
		{
			goodFeaturesToTrack(gray, points[0], MAX_POINT_COUNT, 0.01, 20);
		}

		if (lastGray.empty())
			gray.copyTo(lastGray);

		// �������
		calcOpticalFlowPyrLK(lastGray, gray, points[0], points[1], status, err);

		// ����ɾ�������е�
		int counter = 0;
		//temp.clear();
		//temp.insert(temp.end(), points[0].begin(), point)
		for (int i = 0; i < points[1].size(); i++)
		{
			double dist = norm(points[1][i] - points[0][i]);
			if (status[i] && dist >= 2.0 && dist <= 20.0) // ���������׷�ٵ�
			{
				points[0][counter] = points[0][i];
				points[1][counter++] = points[1][i];
			}
				
		}

		points[0].resize(counter);
		points[1].resize(counter);

		// ��ʾ��������˶��켣
		source.copyTo(result);
		for (int i = 0; i < points[1].size(); i++)
		{
			line(result, points[0][i], points[1][i], Scalar(0, 0, 0xff));
			circle(result, points[1][i], 3, Scalar(0, 0xff, 0));
		}

		swap(points[0], points[1]);
		swap(lastGray, gray);

		imshow("Դͼ��", source);
		imshow("�����", result);
		// ���¼���Ƿ���ֹ(����ESC��ֹ����ӦASCII 27)
		char key = waitKey(100);
		if (key == 27)
			break;
	}

}

bool needToInit()
{
	return true;

}