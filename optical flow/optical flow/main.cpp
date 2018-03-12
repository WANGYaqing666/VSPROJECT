/****************************************************/
/****************************************************/
/**************OpenCV Sample Program*****************/
/******************sparse optical flow**************/
/***************WANG Yaqing 12/01/2017***************/
/****************************************************/
/****************************************************/
//读取摄像头文件 并储存
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame, gray;// 当前帧图像
Mat prev_frame, prev_gray;//保存前一帧的图像
vector<Point2f> features;//保存成为一个全局变量 2维float shi-tomasi角点检测 - 特征数据
vector<Point2f> iniPoints;//保存初始特征点得到的数据 初始化特征数据 初始化跟踪点的位置
vector<Point2f> fpts[2];//保存当前帧和前一帧的特征点位置fpts[0]为特征点的原来位置，fpts[1]为特征点的新位置
vector<uchar> status; // 特征点跟踪成功标志位
vector<float> errors; // 跟踪时候区域误差和
vector<Point2f> pt1, pt2;

void detectFeatures(Mat &inFrame, Mat &ingray);//定义一个方法角点检测
void drawFeature(Mat &inFrame);
void klTrackFeature();//track
void drawTrackLines();


int main(int argc, char** argv)
{
	//VideoCapture capture(0);
	VideoCapture capture;
	capture.open("E:/openCV/video/1.AVI");
	if(!capture.isOpened())
	{
		printf("could not load video file...\n");
		return -1;

	}
	
	double fps = capture.get(CV_CAP_PROP_FPS);
	namedWindow("optical flow", CV_NORMAL);
	Size size = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("FPS;%f", fps);
	//VideoWriter writer("E:/openCV/video/wv_demo6.avi", -1, 15.0, size, true);
	while (capture.read(frame))//获取第一帧图像
	{
		flip(frame, frame, 1);
		cvtColor(frame, gray, COLOR_BGRA2GRAY);//当前帧保存为前一帧 特征检测基于灰度图像 所以把彩色图像变为灰度图像COLOR_BGRA2GRAY
		if (fpts[0].size() < 40)//特征点数目小于40重新跟踪 采样得到新的特征点 放到前一帧fpts[0]的数据中去
		{
           detectFeatures(frame, gray);//特征检测Harris或者shi-tomashi
		   fpts[0].insert(fpts[0].end(), features.begin(), features.end());//数据保存 在fpts[0]的最后插入feature的第一个和最后一个的下一个 把新的数据放在以前数据的结尾 fpts[0].end()是最后的下一个 insert是插入最后的前一个
		   iniPoints.insert(iniPoints.end(), features.begin(), features.end());//  让你真的好像能看到光流 
		}
		else
		{
			printf("tttttttttttttttttttttttttttttttttttttttt...\n");//打印t就是在check
         }

		if (prev_gray.empty())//看前一帧当中prev_gray 是不是为空 如果为空说明是第一帧 若为第一帧拷贝
		{
			gray.copyTo(prev_gray);
		}

		klTrackFeature();//定义一个KLTtrack
		drawFeature(frame);//把特征点画出来 把检测到的特征点画到frame上去

        // 更新前一帧数据
		gray.copyTo(prev_gray);//把当前帧拷贝的前一帧中
		frame.copyTo(prev_frame);//当前frame拷贝到前一帧frame中


		imshow("optical flow", frame);
		//writer.write(frame);//写视频
		//视频break
		char c = waitKey(1);//每隔50毫秒去读一帧
		if (c == 27)
		{
			break;
		}
	}
	capture.release();
	waitKey(0);
	return 0;
}
void detectFeatures(Mat &inFrame, Mat &ingray)//定义一个方法角点检测
{
	double maxCorners = 5000;//检测出来的角点最多是5000
	double qualitylevel = 0.01;//符合条件的
	double minDistance = 10;//两个特征点之间最小距离 如果小于101认为是同一个特征点
	double blockSize = 3;//3X3
	double k = 0.04;//阈值

	goodFeaturesToTrack(ingray, features, maxCorners, qualitylevel, minDistance, Mat(), blockSize, false, k);//false shi—tomashi角点检测这个快；k默认是0.04
	cout << "detect features : " << features.size() << endl;//跟踪的时候显示detect features

}

void drawFeature(Mat &inFrame)
{
	for (size_t t = 0; t < fpts[0].size(); t++)
	{
		circle(inFrame, fpts[0][t], 2, Scalar(0, 0, 255), 2, 8, 0);//对每个特征点画圆，圆的中心是每个特征点的中心features[t] 半径2；红色
	}
}
void klTrackFeature()//track
{
	// KLT LKT 稀疏光流
	calcOpticalFlowPyrLK(prev_gray, gray, fpts[0], fpts[1], status, errors);//以前的灰度图像；当前帧的灰度图像；以前的特征点；当前的特征点；成功；错误；后面大部分是默认的
	//输入特征点100个 输出特征点80个 其中有20个丢失了但是status中还是会给我们100个值
	int k = 0;
	// 特征点过滤
	for (int i = 0; i < fpts[1].size(); i++)
	{
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);//计算特征点之间的距离abs绝对值函数
		if (dist > 2 && status[i])//如果距离大于2并且有状态（既被标记了，跟踪成功了）
		{
			iniPoints[k] = iniPoints[i];//把初始化状态切掉 重新初始化
			fpts[1][k++] = fpts[1][i];
		}
	}
	// 保存特征点并绘制跟踪轨迹
	iniPoints.resize(k);//
	fpts[1].resize(k);
	drawTrackLines();
	std::swap(fpts[1], fpts[0]);//fpts[1]和fpts[0]交换 拷贝到前一帧当中去 以备下一帧来的时候我们还可以继续进行比较

}




void drawTrackLines() 
{
	for (size_t t = 0; t < fpts[1].size(); t++)
	{
		
			line(frame, iniPoints[t], fpts[1][t], Scalar(0, 255, 0), 1, 8, 0);//绿色的线
			circle(frame, fpts[1][t], 2, Scalar(0, 0, 255), 2, 8, 0);//红色的点 RNG random一下
		}
	}

