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
    createTrackbar("Max ����", "img", &MaxValue, 200, onChangeValue);
    createTrackbar("Min ����", "img", &MinValue, 200, onChangeValue);

    imageShow();
    waitKey(0);
    return 0;
}

// Ʈ����
void onChangeValue(int value, void* userData)
{
    imageShow();
}

// ��
void setLabel(Mat& img, const vector<Point>& pts, const String& label)
{
    Rect rc = boundingRect(pts);// �������� �ڽ� 
    Point pt = Point((rc.x + rc.width / 2), rc.y + rc.height / 2);
    circle(img, pt, 3, Scalar(0, 0, 255), -1); // �߽���ǥ�� ��Ŭ��ȯ
}

void imageShow()
{
    dst = img.clone();

    Mat test;
    cvtColor(img, test, COLOR_BGR2GRAY);
    threshold(test, test, 173, 255, THRESH_BINARY); // �������
    Matx <uchar, 3, 3>mask(0, 1, 0, 1, 1, 1, 0, 1, 0);
    morphologyEx(test, test, MORPH_DILATE, mask); // ħ��
    //imshow("test", test);

    Mat bin;
    cvtColor(img, bin, COLOR_BGR2GRAY);
    threshold(bin, bin, 104, 255, THRESH_BINARY_INV | THRESH_OTSU); // ���������� �������
    morphologyEx(bin, bin, MORPH_DILATE, mask); // ħ��
    //imshow("bin", bin);
    test = test | bin; // �ΰ��� ���� OR
    //imshow("or", test);
    Mat img_color; // 3ä�� �������� ǥ���ϱ� ���� �̹��� ����
    cvtColor(test, img_color, COLOR_GRAY2BGR);


    vector<vector<Point>> contours;
    findContours(test, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // ���ø�� ���ͼ��� ���̱�����
    for (vector<Point> pts : contours) {
        double areaValue = contourArea(pts); // ������ ���

        // max���� min���� �־� ���� �������� �ʿ���� ������ ����
        if (areaValue > MaxValue || areaValue < MinValue) 
            continue;
        setLabel(dst, pts, ""); // ���� ���
    }
    imshow("img", dst);
}

