//����ͷ��Ƶ���� ������ʾ
#include<opencv2/opencv.hpp> //��������ͷ�ļ�
#include<iostream>//����iostreamͷ�ļ�

using namespace cv;//�涨����λ�ڴ������ռ�֮��
using namespace std;//��׼�������

int main(int argc, char**argv)
{
	//VideoCapture capture;//������Ƶ�ļ�
	//capture.open("E:/openCV/video/video_003.avi");//��׼�������stream ���߷ŵ�main�����£�˫������д��Ƶ���֡���ʽ
	VideoCapture capture(0);
	if (!capture.isOpened())// �ǲ���
	{
		printf("could not load video data...\n");//  û�д򿪣���������-1
		return -1;

	}
	double fps = capture.get(CV_CAP_PROP_FPS);
	Size size = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("FPS;%f", fps);

	//VideoWriter writer("E:/openCV/video/wv_demo.mp4",-1,fps,size,true); mp4�������� -1�Զ��� fps���
	//VideoWriter writer("E:/openCV/video/wv_demo.mp4", CV_FOURCC('D', 'I', 'V', 'X'), 15.0, size, true);15֡ÿ��
	Mat frame, gray, binary;//һ��������ѭ��֮��
	namedWindow("video-demo", CV_WINDOW_AUTOSIZE);// WINDOW_NORMAL�û������ֶ��ı䴰�ڵĴ�СCV_WINDOW_AUTOSIZE�����Զ������û������ֶ��ı�CV_WINDOW_OPENGL֧��OPENGL
	while (capture.read(frame))//��ȡ��Ƶ read��һ֡һ֡��ȥ����Ƶ ���û�з���none
	{
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);//���һ�� ��ȡ��ֵ��|�Զ���ֵ����ȡ
		//bitwise_not(frame, frame);
		imshow("video-demo", binary);//��ÿһ֡ͼ����ʾ����

		//writer.write(frame);
		char c = waitKey(100);//ÿ��100����ȥ��һ֡
		if (c == 27) //����Esc��ֹͣ
		{
			break;
		}
	}

	waitKey(0);//�ȴ�ʱ������� �����������
	return 0;
}




