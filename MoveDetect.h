#ifndef MoveDetect_H
#define MoveDetect_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/flann/flann.hpp"
#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/gpu/gpu.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace cv;
using namespace std;

class MoveDetect
{
public:
	MoveDetect();
	VideoCapture capture;

	Mat frame2;
	Mat frame1;

	Mat  currROI, prevROI;
	Mat hann;
	Mat prev64f, curr64f;
	

	vector <Point2f> features1, features2;
	vector <Point2f> goodFeatures1, goodFeatures2;
	vector <uchar> status;
	vector <float> err;

	double dx;
	double dy;
	double da;


	bool result_label;
	bool phase_label;
	bool Itfirst;
	int IsMove;
	int IsNoMove;
	int computtimes;
	float ratio;
	int angle;

	void Detect(Mat frame_1, Mat frame_2, bool& result);
	void Change_position(Mat& img);
	Mat extract_ROI(Mat input, float prop_row, float prop_col);
};



#endif // VIDEOSTAB_H
