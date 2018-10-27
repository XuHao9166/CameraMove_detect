#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/flann/flann.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>
#include "MoveDetect.h"

using namespace std;
using namespace cv;




int main(int argc, char **argv)
{

	//创建视频稳定类的实例
	MoveDetect detector;
	bool end ;

	//VideoCapture cap(1);
	VideoCapture cap("E:\\相机移动视频\\高处\\3.mp4");

	Mat frame_2, frame2;
	Mat frame_1, frame1;


	long frameToStart = 10;  ///跳过前十帧
	cap.set(CV_CAP_PROP_POS_FRAMES, frameToStart);

	cap >> frame_1;
	cvtColor(frame_1, frame1, COLOR_BGR2GRAY);

	while (true)
	{
		end = false;
		cap >> frame_2;

		if (frame_2.data == NULL)
		{
			continue;
		}

		cvtColor(frame_2, frame2, COLOR_BGR2GRAY);

		

		detector.Detect(frame_1, frame_2,end);

		if (end)
		{
			cout << end << endl;
		}
		

		

		imshow("src", frame_2);

		frame_1 = frame_2.clone();
		//frame2.copyTo(frame1);
		waitKey(30);
	}

	system("pause");

	return 0;
}


