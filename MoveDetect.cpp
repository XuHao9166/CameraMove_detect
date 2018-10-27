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

	Mat q0(img, Rect(0, 0, cx, cy)); // ����
	Mat q1(img, Rect(cx, 0, cx, cy)); // ����
	Mat q2(img, Rect(0, cy, cx, cy)); // ���� 
	Mat q3(img, Rect(cx, cy, cx, cy)); //���� 

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3); // �������Һ������λ��

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



//��Ҫ�Ĵ�����
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


	///�޶���������������
	Mat mask = Mat::zeros(frame1.size(), CV_8UC1);
	

/////����mask
	float  prop_row = 0.4;   ////��ֵҪС��0.5 
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


///////��ͼƬ�ֿ鲢����λ��
	Change_position(mask);
	Change_position(frame1);
	Change_position(frame2);

	/*frame1.copyTo(prevROI, mask);   ///���к�ɫ����
	frame2.copyTo(currROI, mask);*/
	
	prevROI = extract_ROI(frame1,  prop_row,  prop_col);  //��ȡROI
	currROI = extract_ROI(frame2, prop_row, prop_col);

	prevROI.convertTo(prev64f, CV_64F);
	currROI.convertTo(curr64f, CV_64F);

/////////������Ŀ�������λ�����������Ŀ�ͼ����˶������������
	Point2d shift = phaseCorrelate(prev64f, curr64f, hann);  //������������
	double radius = std::sqrt(shift.x*shift.x + shift.y*shift.y); ///����뾶���ɰ뾶��С�����˶����ҳ̶�
	
	cout << "radiu:"<<radius << endl;

	if (  2.5 < radius   )
	{
		/////���㷽���
		Mat mask2 = Mat::zeros(frame1.size(), CV_8UC1);
		Point end_point;
		Point center(mask2.cols >> 1, mask2.rows >> 1);
		circle(mask2, center, (int)radius, Scalar(255, 255, 255), 3, LINE_AA);
		Mat grad1, grad2, angle_img;
		Sobel(mask2, grad1, CV_64FC1, 1, 0); //���ݶ�
		Sobel(mask2, grad2, CV_64FC1, 0, 1);
		blur(grad1, grad1, Size(6, 6));
		blur(grad2, grad2, Size(6, 6));
		phase(grad1, grad2, angle_img, true);
		normalize(angle_img, angle_img, 0, 255, NORM_MINMAX); //��һ������ɷ�Χ��0��255������ֵ�� ������ʾ����ʵ������û�й�ϵ
		angle_img.convertTo(angle_img, CV_8UC1);

		end_point = Point(center.x + (int)shift.x - 3, center.y + (int)shift.y - 3);
		//cout << end_point.x << "   " << end_point.y << endl;

		angle = angle_img.at<uchar>(end_point.y, end_point.x);

		cout <<"angle:"<< angle << endl;

		//�����ļ��㹫ʽ
		// ����: x/255 = (30��330��150��210)/360  ��x
		if (angle< 45 || angle > 211)    ///������30��Ϊ��ֵ
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


