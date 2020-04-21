#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
void onChangeValue(int value, void* userData);
void setLabel(Mat& img, const vector<Point>& pts, const String& label);
void imageShow();

Mat img;
Mat dst;
int MaxValue = 98;
int MinValue = 2;

int main(int argc, char* argv[])
{
    Mat ROI;
    img = imread("images/20200115220907181 CAM_3 ULC.png", IMREAD_COLOR);

    //Canny(img,)
    if (img.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    namedWindow("img", WINDOW_AUTOSIZE);
    createTrackbar("Max 변경", "img", &MaxValue, 200, onChangeValue);
    createTrackbar("Min 변경", "img", &MinValue, 200, onChangeValue);

    imageShow();
    waitKey(0);
    return 0;
}

// 트랙바
void onChangeValue(int value, void* userData)
{
    imageShow();
}

// 원
void setLabel(Mat& img, const vector<Point>& pts, const String& label)
{
    Rect rc = boundingRect(pts);// 컨투어의 박스 
    Point pt = Point((rc.x + rc.width / 2), rc.y + rc.height / 2);
    circle(img, pt, 3, Scalar(0, 0, 255), -1); // 중심좌표로 서클변환
}

void imageShow()
{
    dst = img.clone();

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
    //imshow("or", test);
    Mat img_color; // 3채널 빨간점을 표기하기 위한 이미지 생성
    cvtColor(test, img_color, COLOR_GRAY2BGR);


    vector<vector<Point>> contours;
    findContours(test, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // 심플모드 백터수를 줄이기위해
    for (vector<Point> pts : contours) {
        double areaValue = contourArea(pts); // 면적값 계산

        // max값과 min값을 주어 원의 면적으로 필요없는 연산을 제외
        if (areaValue > MaxValue || areaValue < MinValue) 
            continue;
        setLabel(dst, pts, ""); // 점을 찍기
    }
    imshow("img", dst);
}

