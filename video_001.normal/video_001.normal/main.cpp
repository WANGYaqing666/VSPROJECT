//����ͷ��Ƶ���� ������Ƶ�ļ�
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
	//��ȡ֡������
	double fps = capture.get(CV_CAP_PROP_FPS);
	Size size = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("FPS;%f", fps);

	VideoWriter writer("E:/openCV/video/wv_demo8.avi",-1,15.0,size,true); //mp4��������;�ڶ�������fcc-1�Զ���;fps���15.0ÿ��15֡ ��Ƶд��û������ ��Ƶ�ļ����2G�ٴ��������;true�����ɫ
	//VideoWriter writer("E:/openCV/video/wv_demo.mp4", CV_FOURCC('D', 'I', 'V', 'X'), 15.0, size, true);//15֡ÿ��
	Mat frame;//һ��������ѭ��֮��
	namedWindow("video-demo8", CV_WINDOW_AUTOSIZE);// WINDOW_NORMAL�û������ֶ��ı䴰�ڵĴ�СCV_WINDOW_AUTOSIZE�����Զ������û������ֶ��ı�CV_WINDOW_OPENGL֧��OPENGL
	//show each frame and save
	while (capture.read(frame))//��ȡ��Ƶ read��һ֡һ֡��ȥ����Ƶ ���û�з���none
	{
		imshow("video-demo8", frame);//��ÿһ֡ͼ����ʾ����
        writer.write(frame);
		char c = waitKey(100);//ÿ��100����ȥ��һ֡
		if (c == 27) //����Esc��ֹͣ
		{
			break;
		}
	}
	capture.release();
	waitKey(0);//�ȴ�ʱ������� �����������
	return 0;
}




