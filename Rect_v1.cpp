#include "opencv2/opencv.hpp" 
#include <iostream>  
//#include <string> 
using namespace cv;
using namespace std;

Point2f ptx;
//Contour ���� ���� �ؽ�Ʈ ���� 
//https://github.com/bsdnoobz/opencv-code/blob/master/shape-detect.cpp
void setLabel(Mat& image, string str, vector<Point> contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.5;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), FILLED);
	putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
	/*for (Point pts : contour) {
		cout << pts.x << ", " << pts.y << endl;
	}*/
}



int main(int, char**)
{
	Mat img, img_result, img_gray;

	//�̹��������� �ε��Ͽ� image�� ����  
	img = imread("image/a1.png", IMREAD_COLOR);
	if (img.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}


	Mat dst = img.clone();

	Mat test;
	cvtColor(img, test, COLOR_BGR2GRAY);
	threshold(test, test, 22, 255, THRESH_BINARY); // �������
	Matx <uchar, 3, 3>mask(0, 1, 0, 1, 1, 1, 0, 1, 0);
	morphologyEx(test, test, MORPH_DILATE, mask); // ħ��
	//imshow("test", test);

	Mat bin;
	cvtColor(img, bin, COLOR_BGR2GRAY);
	threshold(bin, bin, 78, 255, THRESH_BINARY_INV | THRESH_OTSU); // ���������� �������
	morphologyEx(bin, bin, MORPH_DILATE, mask); // ħ��
	//imshow("bin", bin);
	bitwise_not(bin, bin);
	test = test | bin; // �ΰ��� ���� OR

	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	medianBlur(img_gray, img_gray, 5);
	Mat blur;
	//medianBlur(test, blur, 5);
	//bitwise_not(test, blur);
	//bitwise_or(test, bin, blur);
	//bitwise_not(test, test);
	//morphologyEx(test, blur, MORPH_CLOSE, mask);

	//�׷��̽����� �̹����� ��ȯ  
	//cvtColor(img_input, img_gray, COLOR_BGR2GRAY);
	/*Mat image_canny;
	blur(img_gray, image_canny, Size(3, 3));
	Canny(image_canny, image_canny, 100, 100, 3);*/

	//����ȭ �̹����� ��ȯ
	//Mat binary_image;
	//threshold(img_gray, binary_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

//	Mat binary_image1;
//	threshold(img_gray, binary_image1, 150, 255, THRESH_TOZERO_INV);
	//�� ���� ����ȭ (���ڶ� ���ֺ��� ����)
//	Mat binary_image2;
	//threshold(binary_image1, binary_image2, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);


	//medianBlur(img_gray, img_gray, 5);
	
	//contour�� ã�´�.
	vector<vector<Point> > contours;
	findContours(test, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	
	//contour�� �ٻ�ȭ�Ѵ�.
	vector<Point2f> approx;
	img_result = img.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.002, true);

		if (fabs(contourArea(Mat(approx))) > 10000)  //������ ����ũ�� �̻��̾�� �Ѵ�. 
		{

			int size = approx.size();
			
			//Contour�� �ٻ�ȭ�� ������ �׸���.
			if (size % 2 == 0) {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size; k++)
				{
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
					cout << approx[k] << endl;
				}
				for (Point2f pt : approx) {
					if (pt.x > 0 && pt.y > 0) {
						if (pt.x < img_result.cols - 1 && pt.y < img_result.rows - 1)
							ptx = pt;
					}

				}

			}
			else {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 1);

				
				for (int k = 0; k < size; k++)
				{
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
					cout << approx[k] << endl;
				
					

				}
			}

			Rect roi(ptx.x+15, 0, 13, ptx.y);
			Rect roi1(ptx.x, ptx.y-25, img_result.cols - ptx.x, 15);
			rectangle(img_result, roi, Scalar(0, 0, 255), 1);
			rectangle(img_result, roi1, Scalar(0, 0, 255), 1);
			vector<Vec3f> circles;
			vector<Vec3f> circles1;
			HoughCircles(img_gray(roi), circles, HOUGH_GRADIENT, 1, 3, 50, 5, 1, 1);

			//HoughCircles(img_gray(roi1), circles1, HOUGH_GRADIENT, 1, 3, 30, 5, 1, 1);

			for (size_t i = 0; i < circles.size(); i++)
			{
				Vec3i c = circles[i];
				Point center(c[0] + ptx.x+15, c[1]);
				int radius = c[2];

				circle(img_result, center, radius, Scalar(0, 255, 0), 2);
				circle(img_result, center, 2, Scalar(0, 0, 255), 3);
			}

			/*for (size_t i = 0; i < circles1.size(); i++)
			{
				Vec3i c = circles1[i];
				Point center(c[0] + ptx.x, c[1] + ptx.y-25);
				int radius = c[2];

				circle(img_result, center, radius, Scalar(0, 255, 0), 2);
				circle(img_result, center, 2, Scalar(0, 0, 255), 3);
			}*/


			//������ �����Ѵ�.
			if (size == 3)
				setLabel(img_result, "triangle", contours[i]); //�ﰢ��

			//���ϴ� �ش� �������� ���� convex��� ã�� ����
			else if (size == 4 && isContourConvex(Mat(approx)))
				setLabel(img_result, "rectangle", contours[i]); //�簢��

			else if (size == 5 && isContourConvex(Mat(approx)))
				setLabel(img_result, "pentagon", contours[i]); //������

			else if (size == 6 && isContourConvex(Mat(approx)))
				setLabel(img_result, "hexagon", contours[i]);  //������

			else if (size == 10 && isContourConvex(Mat(approx)))
				setLabel(img_result, "decagon", contours[i]);    //�ʰ���

			//�� ���ǿ� �ش� �ȵǴ� ���� ã�Ƴ� ������ ������ ǥ��
			//else setLabel(img_result, to_string(approx.size()), contours[i]);
		}
	}
	//string title2 = "NORMAL";
	//namedWindow(title2, WINDOW_NORMAL);
	//
	//resizeWindow(title2, 800, 900);	//ũ�� ���� �Ұ� - AUTOSIZE��
	
	imshow("sex", test);
	imshow("���", img_result);
	//imshow(title2, img_result);

	waitKey(0);

	return 0;
}

