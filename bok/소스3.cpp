#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 

using namespace cv;
using namespace std;



//Contour 영역 내에 텍스트 쓰기 
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
}



int main(int, char**)
{
	Mat img, img_result, img_gray;

	//이미지파일을 로드하여 image에 저장  
	img = imread("image/a1.png", IMREAD_COLOR);
	if (img.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}


	Mat dst = img.clone();

	Mat test;
	cvtColor(img, test, COLOR_BGR2GRAY);
	threshold(test, test, 173, 255, THRESH_BINARY); // 쓰레숄드
	Matx <uchar, 3, 3>mask(0, 1, 0, 1, 1, 1, 0, 1, 0);
	morphologyEx(test, test, MORPH_DILATE, mask); // 침식
	//imshow("test", test);

	Mat bin;
	cvtColor(img, bin, COLOR_BGR2GRAY);
	threshold(bin, bin, 104, 255, THRESH_BINARY_INV | THRESH_OTSU); // 리버스값을 쓰레숄드
	morphologyEx(bin, bin, MORPH_DILATE, mask); // 침식
	//imshow("bin", bin);
	test = test | bin; // 두개의 값을 OR


	//그레이스케일 이미지로 변환  
	//cvtColor(img_input, img_gray, COLOR_BGR2GRAY);
	/*Mat image_canny;
	blur(img_gray, image_canny, Size(3, 3));
	Canny(image_canny, image_canny, 100, 100, 3);*/

	//이진화 이미지로 변환
	//Mat binary_image;
	//threshold(img_gray, binary_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

//	Mat binary_image1;
//	threshold(img_gray, binary_image1, 150, 255, THRESH_TOZERO_INV);
	//그 다음 이진화 (끝자락 없애보기 위해)
//	Mat binary_image2;
	//threshold(binary_image1, binary_image2, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);


	//medianBlur(img_gray, img_gray, 5);
	
	//contour를 찾는다.
	vector<vector<Point> > contours;
	findContours(test, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	
	//contour를 근사화한다.
	vector<Point2f> approx;
	img_result = img.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.002, true);

		if (fabs(contourArea(Mat(approx))) > 100)  //면적이 일정크기 이상이어야 한다. 
		{

			int size = approx.size();

			//Contour를 근사화한 직선을 그린다.
			if (size % 2 == 0) {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}
			else {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 1);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}



			//도형을 판정한다.
			if (size == 3)
				setLabel(img_result, "triangle", contours[i]); //삼각형

			//이하는 해당 꼭지점을 가진 convex라면 찾는 도형
			else if (size == 4 && isContourConvex(Mat(approx)))
				setLabel(img_result, "rectangle", contours[i]); //사각형

			else if (size == 5 && isContourConvex(Mat(approx)))
				setLabel(img_result, "pentagon", contours[i]); //오각형

			else if (size == 6 && isContourConvex(Mat(approx)))
				setLabel(img_result, "hexagon", contours[i]);  //육각형

			else if (size == 10 && isContourConvex(Mat(approx)))
				setLabel(img_result, "decagon", contours[i]);    //십각형

			//위 조건에 해당 안되는 경우는 찾아낸 꼭지점 갯수를 표시
			else setLabel(img_result, to_string(approx.size()), contours[i]);
		}
	}
	
	imshow("input", test);
	imshow("result", img_result);

	waitKey(0);

	return 0;
}