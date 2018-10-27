#include "MoveDetect.h"
#include <cmath>


MoveDetect::MoveDetect()
{
	result_label = false;
	phase_label = false;
	Itfirst = false;
	IsMove = 0;
	IsNoMove = 0;
	computtimes = 0;
	ratio = 0;
	angle = 0;

}

void MoveDetect::Change_position(Mat& img)
{
	int cx = img.cols / 2;
	int cy = img.rows / 2;

	Mat q0(img, Rect(0, 0, cx, cy)); // 左上
	Mat q1(img, Rect(cx, 0, cx, cy)); // 右上
	Mat q2(img, Rect(0, cy, cx, cy)); // 左下 
	Mat q3(img, Rect(cx, cy, cx, cy)); //右下 

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3); // 调换上右和下左的位置

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

}

Mat MoveDetect::extract_ROI(Mat input, float prop_row, float prop_col)
{
	int center_x = input.cols / 2;
	int center_y = input.rows / 2;

	int x = center_x - input.cols*prop_col;
	int y = center_y - input.rows*prop_row;

	int x_end = center_x + input.cols*prop_col;
	int y_end = center_y + input.rows*prop_row;

	Mat imgROI = input(Rect( Point(x,y),Point(x_end,y_end)));

	return imgROI;
}



//主要的处理函数
void MoveDetect::Detect(Mat frame_1, Mat frame_2 ,bool& result)
{
	cvtColor(frame_1, frame1, COLOR_BGR2GRAY);
	cvtColor(frame_2, frame2, COLOR_BGR2GRAY);

	result_label = false;
	phase_label = false;
	result = false;
	angle = 0;

	features1.clear(); 
	features2.clear();
	goodFeatures1.clear(),
	goodFeatures2.clear();
	status.clear();
	err.clear();


	///限定获得特征点的区域
	Mat mask = Mat::zeros(frame1.size(), CV_8UC1);
	

/////矩形mask
	float  prop_row = 0.4;   ////该值要小于0.5 
	float  prop_col = 0.1;

	for (int m = 0; m < frame1.rows; m++)
	{
		uchar *p = mask.ptr<uchar>(m);

		for (int n = 0; n < frame1.cols; n++)
		{
			/*cout << frame1.rows*prop_row << endl;
			cout << frame1.cols*prop_col << endl;
			cout << n << endl;*/
			if (m <= (frame1.rows*prop_row))
			{
				if ( n <= frame1.cols* prop_col || frame1.cols*(1- prop_col) <= n )
				{
					p[n] = 255;
				}
			}
			
			if (frame1.rows*(1 - prop_row) <= m )
			{

				if ( n <= frame1.cols* prop_col || frame1.cols*(1 - prop_col) <= n )
				{
					p[n] = 255;
				}

			}


		}
	}

////////////////////////////////


///////对图片分块并调整位置
	Change_position(mask);
	Change_position(frame1);
	Change_position(frame2);

	/*frame1.copyTo(prevROI, mask);   ///带有黑色部分
	frame2.copyTo(currROI, mask);*/
	
	prevROI = extract_ROI(frame1,  prop_row,  prop_col);  //抽取ROI
	currROI = extract_ROI(frame2, prop_row, prop_col);

	prevROI.convertTo(prev64f, CV_64F);
	currROI.convertTo(curr64f, CV_64F);

/////////计算核心块的相关相位，对整个核心块图像的运动情况进行量化
	Point2d shift = phaseCorrelate(prev64f, curr64f, hann);  //创建汉明窗口
	double radius = std::sqrt(shift.x*shift.x + shift.y*shift.y); ///计算半径，由半径大小量化运动剧烈程度
	
	cout << "radiu:"<<radius << endl;

	if (  2.5 < radius   )
	{
		/////计算方向角
		Mat mask2 = Mat::zeros(frame1.size(), CV_8UC1);
		Point end_point;
		Point center(mask2.cols >> 1, mask2.rows >> 1);
		circle(mask2, center, (int)radius, Scalar(255, 255, 255), 3, LINE_AA);
		Mat grad1, grad2, angle_img;
		Sobel(mask2, grad1, CV_64FC1, 1, 0); //求梯度
		Sobel(mask2, grad2, CV_64FC1, 0, 1);
		blur(grad1, grad1, Size(6, 6));
		blur(grad2, grad2, Size(6, 6));
		phase(grad1, grad2, angle_img, true);
		normalize(angle_img, angle_img, 0, 255, NORM_MINMAX); //归一化（变成范围是0到255的像素值） 方便显示，和实际数据没有关系
		angle_img.convertTo(angle_img, CV_8UC1);

		end_point = Point(center.x + (int)shift.x - 3, center.y + (int)shift.y - 3);
		//cout << end_point.x << "   " << end_point.y << endl;

		angle = angle_img.at<uchar>(end_point.y, end_point.x);

		cout <<"angle:"<< angle << endl;

		//参数的计算公式
		// 例如: x/255 = (30或330或150或210)/360  求x
		if (angle< 45 || angle > 211)    ///俯仰角30度为阈值
		{
			result = true;
		}

		if (angle > 83 && angle < 172)
		{
			result = true;
		}
		   
	}

	imshow("currROI", currROI);

///////////////////////////////////////////////////



}


