/****************************************************/
/****************************************************/
/**************OpenCV Sample Program*****************/
/******************sparse optical flow**************/
/***************WANG Yaqing 12/01/2017***************/
/****************************************************/
/****************************************************/
//��ȡ����ͷ�ļ� ������
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame, gray;// ��ǰ֡ͼ��
Mat prev_frame, prev_gray;//����ǰһ֡��ͼ��
vector<Point2f> features;//�����Ϊһ��ȫ�ֱ��� 2άfloat shi-tomasi�ǵ��� - ��������
vector<Point2f> iniPoints;//�����ʼ������õ������� ��ʼ���������� ��ʼ�����ٵ��λ��
vector<Point2f> fpts[2];//���浱ǰ֡��ǰһ֡��������λ��fpts[0]Ϊ�������ԭ��λ�ã�fpts[1]Ϊ���������λ��
vector<uchar> status; // ��������ٳɹ���־λ
vector<float> errors; // ����ʱ����������
vector<Point2f> pt1, pt2;

void detectFeatures(Mat &inFrame, Mat &ingray);//����һ�������ǵ���
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
	while (capture.read(frame))//��ȡ��һ֡ͼ��
	{
		flip(frame, frame, 1);
		cvtColor(frame, gray, COLOR_BGRA2GRAY);//��ǰ֡����Ϊǰһ֡ ���������ڻҶ�ͼ�� ���԰Ѳ�ɫͼ���Ϊ�Ҷ�ͼ��COLOR_BGRA2GRAY
		if (fpts[0].size() < 40)//��������ĿС��40���¸��� �����õ��µ������� �ŵ�ǰһ֡fpts[0]��������ȥ
		{
           detectFeatures(frame, gray);//�������Harris����shi-tomashi
		   fpts[0].insert(fpts[0].end(), features.begin(), features.end());//���ݱ��� ��fpts[0]��������feature�ĵ�һ�������һ������һ�� ���µ����ݷ�����ǰ���ݵĽ�β fpts[0].end()��������һ�� insert�ǲ�������ǰһ��
		   iniPoints.insert(iniPoints.end(), features.begin(), features.end());//  ������ĺ����ܿ������� 
		}
		else
		{
			printf("tttttttttttttttttttttttttttttttttttttttt...\n");//��ӡt������check
         }

		if (prev_gray.empty())//��ǰһ֡����prev_gray �ǲ���Ϊ�� ���Ϊ��˵���ǵ�һ֡ ��Ϊ��һ֡����
		{
			gray.copyTo(prev_gray);
		}

		klTrackFeature();//����һ��KLTtrack
		drawFeature(frame);//�������㻭���� �Ѽ�⵽�������㻭��frame��ȥ

        // ����ǰһ֡����
		gray.copyTo(prev_gray);//�ѵ�ǰ֡������ǰһ֡��
		frame.copyTo(prev_frame);//��ǰframe������ǰһ֡frame��


		imshow("optical flow", frame);
		//writer.write(frame);//д��Ƶ
		//��Ƶbreak
		char c = waitKey(1);//ÿ��50����ȥ��һ֡
		if (c == 27)
		{
			break;
		}
	}
	capture.release();
	waitKey(0);
	return 0;
}
void detectFeatures(Mat &inFrame, Mat &ingray)//����һ�������ǵ���
{
	double maxCorners = 5000;//�������Ľǵ������5000
	double qualitylevel = 0.01;//����������
	double minDistance = 10;//����������֮����С���� ���С��101��Ϊ��ͬһ��������
	double blockSize = 3;//3X3
	double k = 0.04;//��ֵ

	goodFeaturesToTrack(ingray, features, maxCorners, qualitylevel, minDistance, Mat(), blockSize, false, k);//false shi��tomashi�ǵ�������죻kĬ����0.04
	cout << "detect features : " << features.size() << endl;//���ٵ�ʱ����ʾdetect features

}

void drawFeature(Mat &inFrame)
{
	for (size_t t = 0; t < fpts[0].size(); t++)
	{
		circle(inFrame, fpts[0][t], 2, Scalar(0, 0, 255), 2, 8, 0);//��ÿ�������㻭Բ��Բ��������ÿ�������������features[t] �뾶2����ɫ
	}
}
void klTrackFeature()//track
{
	// KLT LKT ϡ�����
	calcOpticalFlowPyrLK(prev_gray, gray, fpts[0], fpts[1], status, errors);//��ǰ�ĻҶ�ͼ�񣻵�ǰ֡�ĻҶ�ͼ����ǰ�������㣻��ǰ�������㣻�ɹ������󣻺���󲿷���Ĭ�ϵ�
	//����������100�� ���������80�� ������20����ʧ�˵���status�л��ǻ������100��ֵ
	int k = 0;
	// ���������
	for (int i = 0; i < fpts[1].size(); i++)
	{
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);//����������֮��ľ���abs����ֵ����
		if (dist > 2 && status[i])//����������2������״̬���ȱ�����ˣ����ٳɹ��ˣ�
		{
			iniPoints[k] = iniPoints[i];//�ѳ�ʼ��״̬�е� ���³�ʼ��
			fpts[1][k++] = fpts[1][i];
		}
	}
	// ���������㲢���Ƹ��ٹ켣
	iniPoints.resize(k);//
	fpts[1].resize(k);
	drawTrackLines();
	std::swap(fpts[1], fpts[0]);//fpts[1]��fpts[0]���� ������ǰһ֡����ȥ �Ա���һ֡����ʱ�����ǻ����Լ������бȽ�

}




void drawTrackLines() 
{
	for (size_t t = 0; t < fpts[1].size(); t++)
	{
		
			line(frame, iniPoints[t], fpts[1][t], Scalar(0, 255, 0), 1, 8, 0);//��ɫ����
			circle(frame, fpts[1][t], 2, Scalar(0, 0, 255), 2, 8, 0);//��ɫ�ĵ� RNG randomһ��
		}
	}

