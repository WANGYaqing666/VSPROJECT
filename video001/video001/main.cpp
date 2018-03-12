//����һ����Ƶ�ļ�
#include<opencv2/opencv.hpp> //��������ͷ�ļ�
#include<iostream>//����iostreamͷ�ļ�

using namespace cv;//�涨����λ�ڴ������ռ�֮��
using namespace std;//��׼�������

int main(int argc, char**argv)
{
	VideoCapture capture;//������Ƶ�ļ�
	capture.open("E:/openCV/video/video_003.avi");//��׼�������stream ���߷ŵ�main�����£�˫������д��Ƶ���֡���ʽ
	if (!capture.isOpened())// �ǲ���
	{
		printf("could not load video data...\n");//  û�д򿪣���������-1
		return -1;

	}
	Mat frame;
	namedWindow("video-demo", CV_WINDOW_AUTOSIZE);// WINDOW_NORMAL�û������ֶ��ı䴰�ڵĴ�СCV_WINDOW_AUTOSIZE�����Զ������û������ֶ��ı�CV_WINDOW_OPENGL֧��OPENGL
	while (capture.read(frame))//��ȡ��Ƶ read��һ֡һ֡��ȥ����Ƶ ���û�з���none
	{
		imshow("video-demo", frame);//��ÿһ֡ͼ����ʾ����
		char c=waitKey(100);//ÿ��100����ȥ��һ֡
		if (c == 27) //����Esc��ֹͣ
		{
			break;
		}
	}
	
	waitKey(0);//�ȴ�ʱ������� �����������
	return 0;
}

