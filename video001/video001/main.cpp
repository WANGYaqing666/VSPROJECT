//读入一个视频文件
#include<opencv2/opencv.hpp> //包含所有头文件
#include<iostream>//包含iostream头文件

using namespace cv;//规定程序位于此命名空间之内
using namespace std;//标准输入输出

int main(int argc, char**argv)
{
	VideoCapture capture;//读进视频文件
	capture.open("E:/openCV/video/video_003.avi");//标准输入输出stream 或者放到main函数下，双引号中写视频名字。格式
	if (!capture.isOpened())// 非操作
	{
		printf("could not load video data...\n");//  没有打开，报错，返回-1
		return -1;

	}
	Mat frame;
	namedWindow("video-demo", CV_WINDOW_AUTOSIZE);// WINDOW_NORMAL用户可以手动改变窗口的大小CV_WINDOW_AUTOSIZE窗口自动调整用户不能手动改变CV_WINDOW_OPENGL支持OPENGL
	while (capture.read(frame))//读取视频 read会一帧一帧的去读视频 最后没有返回none
	{
		imshow("video-demo", frame);//把每一帧图像显示出来
		char c=waitKey(100);//每隔100毫秒去读一帧
		if (c == 27) //按下Esc就停止
		{
			break;
		}
	}
	
	waitKey(0);//等待时间无穷大 按任意键结束
	return 0;
}

