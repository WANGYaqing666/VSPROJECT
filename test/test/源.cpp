#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define NUMBER 20

using namespace std;
using namespace cv;

void Show(std::string name, Mat img)
{
	     namedWindow(name, CV_WINDOW_AUTOSIZE);
	     imshow(name, img);
 }


static Scalar randomColor(RNG &rng)
{
	     return Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	 }

void Drawing_Random_Lines(Mat img, char *win_name, RNG& rng)
{
	     int linetype = 8;
	    int w = 400;
	     Point pt1, pt2;
	     int x_1 = 0, x_2 = w;
	
		    for (int i = 0; i < NUMBER; ++i)
		     {
		         pt1.x = rng.uniform(x_1, x_2);
		         pt1.y = rng.uniform(x_1, x_2);
		         pt2.x = rng.uniform(x_1, x_2);
		         pt2.y = rng.uniform(x_1, x_2);
		
			         line(img, pt1, pt2, randomColor(rng), rng.uniform(1, 10), 8);
		         Show(win_name, img);
		     }
	 }


void Display_Random_Text(Mat img, char *win_name, RNG &rng)
{
	     int linetype = 8;
	     int w = 400;
	     int x1 = 0, x2 = w;
	
		     for (int i = 0; i < NUMBER; ++i)
		     {
		         Point org;
		         org.x = rng.uniform(x1, x2);
		        org.y = rng.uniform(x1, x2);
		
			         putText(img, "Testing Text", org, rng.uniform(0, 8), rng.uniform(0, 100)*0.05 + 0.1, randomColor(rng), rng.uniform(1, 10), linetype);
		
			         Show(win_name, img);
		     }
	 }

void Display_Big_End(Mat img, char* win_name, RNG rng)
 {
	     int w = 400;
	     Size textSize = getTextSize("OpenCV Forever", CV_FONT_HERSHEY_COMPLEX, 3, 5, 0);
	     Point org((w - textSize.width) / 2, (w + textSize.height) / 2);
	     int linetype = 8;
	
		     Mat img2;
	
		     for (int i = 0; i < NUMBER; ++i)
		     {
		         img2 = img - Scalar::all(i);
		         putText(img2, "OpenCV Forever", org, CV_FONT_HERSHEY_COMPLEX, 3, Scalar(i, i, 255), 5, linetype);
		
			         Show(win_name, img2);
		     }
	 }

int main()
 {
	     int w = 400;
	     RNG rng(0xFFFFFFFF);
	     char *win_name = "Drawing";
	     Mat img = Mat::zeros(w, w, CV_8UC3);
	
		     Drawing_Random_Lines(img, win_name, rng);
	     //Drawing_Random_Rectangle(img, win_name, rng);
		     //Drawing_Random_Ellipse(img, win_name, rng);
		     //Drawing_Random_Polylines(img, win_name, rng);
		     //Drawing_Random_FilledPolygon(img, win_name, rng);
		     //Drawing_Random_Circle(img, win_name, rng);
		     Display_Random_Text(img, win_name, rng);
	     Display_Big_End(img, win_name, rng);
	
		     waitKey();
	     return 0;
	 }


