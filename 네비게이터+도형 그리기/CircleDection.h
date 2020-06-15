#pragma once
#include "pch.h"
#include <opencv2/opencv.hpp> 
#include <map>
#include <thread>
#include <Windows.h>
#include <time.h>

using namespace std;
using namespace cv;

class CircleDection;

typedef struct CirDetectionParam
{
    CircleDection* This;
    vector<vector<Point>>* contours;
    vector<Point>* cirCenters;
    Mutex* mut;

}cirDetectionParam;

typedef struct CircleValue
{
    int *distance; // ROI ���� ����
    int *radMin; // ����� �ּ� ������
    int *radMax; // ����� �ִ� ������
    int *BGV; // �� Value
    int *thMinValue; // �ܰ� ��ǥ ���� th
    int *thMaxValue; // �ܰ� ��ǥ ���� th
    int *height; // ����
    int *width; // ����
    int *size; // ��ü ũ��
}cirValue;

class CircleDection
{
private:
    Mat src; // �ҽ� �̹��� 
    Mat result; // ��� �̹���
    HBITMAP resultBitmap;
    CirDetectionParam* threadParam; // thread�� ����ü
    CircleValue cirValue;
    vector<Point> cornerPts; // �ܰ� 3�� ��ǥ ��
    vector<Point> vCirCenters; // Vertical�� �߽� ��ǥ���� �� 
    vector<Point> hCirCenters; // Horizontal�� �߽� ��ǥ���� �� 
    vector<Point> vertexPts;   // �ֿܰ� ROI Vertex Points

    
    // Value
    int distance; // ROI ���� ����
    int radMin; // ����� �ּ� ������
    int radMax; // ����� �ִ� ������
    int BGV; // �׷���
    int thMinValue; // �ܰ� ��ǥ ���� th
    int thMaxValue; // �ܰ� ��ǥ ���� th
    int height; // ����
    int width; // ����
    int size; // ��ü ũ��
 
public:

    // ������
    CircleDection();
    ~CircleDection();

    // �ʱ�ȭ
    void Initialize();
    void Init(); 
    //void Init(Mat& src); 

    // �̹��� ����
    //void SelectImage();
    bool SelectImage();

    //����
    void Run(); // �˰��� �κ� ����
    void ALLRun(); // �ʱ�ȭ ~ ��ü ����

    // Image Data Type ��ȯ
    HBITMAP MatToBitmap(Mat& src);
    Mat BitmapToMat(HBITMAP hBmp);

    // set �Լ�
    void SetImage(Mat& src); // �̹��� Set
    void SetThreshValue(int thMinValue, int thMaxValue); // Thresh value set
    void SetDistance(int distance); // ROI ���� ������ set
    void SetCircleValue(int radMin, int radMax, int BGV); // ������ Value set

    // get �Լ�
    Mat GetSourceImage();
    Mat GetResultImage();
    CircleValue* GetCircleValue();
    

    // show �Լ�
    bool ShowSrcImage(); // �ҽ� �̹��� ���
    bool ShowResultImage(); // ��� �̹��� ���

    bool isReady(); // �ҽ� �̹��� �غ� ����


    // �˰��� ���� �κ�
private:
    // 3�� ��ǥ ���� ������
    void GetCornerPoints();
    // ������ Threshold
    void ToZeroThreshold(Mat& src, Mat& dst, int MinValue, int MaxValue);
    // �ܰ��� ����
    void MyContours(Mat& src, vector<vector<Point>>& Points);
    // �󺧸�
    void Labeling(Mat& src, int* Map, vector<Vec3i>& startPoint);
    // �ܰ��� ����
    void Contour(Mat& src, int sx, int sy, vector<Point>& outPoints, int* Map, int value);
    // ��� �Ҽ��� ������ ������ ������ �������� ����
    // �Ÿ� ����  �ɼ� : true == ���� ���� �Ÿ� ,  false == ���� ���� �Ÿ�
    double PerpendicularDistance(const Point& pt, const Point& lineStart, const Point& lineEnd, bool flag);
    // ��� �Ҽ��� ������ ������ ������ �������� ����
    void DouglasPeucker(const vector<Point>& polyLine, vector<Point>& simplifiedPolyLine, double epsilon, bool flag = true, bool endFlag = false);
    // �ֿܰ� ROI Vertex ����
    void GetVertexPoints();
    // �������� ���� ROI Area ����
    void ContourDetection();
    // Points�� ���� �ش� �κ��� �̹��� ����
    void fillPoly_(Size matSize, Mat& dst, vector<Point> pts);
    // �� ���� Thread
    static unsigned WINAPI CirDetectionThread(void* para);
    // �� ����
    void CircleDetection(vector<vector<Point>>& contours, vector<Point>& cirCenters);
    // ��� Point�� �����ϴ� rect ����
    Rect boundRect(vector<Point> pts);
    // �̺� �Լ�(prewitt Edge)
    void Differential(Mat& src, Mat& dst);
    // �簢�� ������ �� ���� ����
    bool IsContain(Rect rc, vector<Point>& cirCenters);
    // �ּ��������� ���� ���� ǥ��, ���� �� �� �׸���
    void Drawing();
    // �ּ������� x, y��ǥ ����Ī �� ����� �� ���� ���� �ٽ� y=x ��Ī�̵�
    Vec2f LSM_Vertical(vector<Point>& pts);
    // �ּ������� �Լ�
    Vec2f LSM_Horizontal(vector<Point>& pts);
};

