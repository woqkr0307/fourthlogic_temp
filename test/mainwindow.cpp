#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include <qlabel.h>
#include <qimage.h>
using namespace cv;
using namespace  std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{

   /*
    Mat image=imread("color_space.jpg",IMREAD_COLOR);

   assert(image.data);
   imshow("im",image);
*/
    imageLabel=new QLabel();

     QString qsfileName = QFileDialog::getOpenFileName(this,
           tr("Open Image"), "../", tr("Image Files (*.png *.jpg *.bmp)"));
       cv::Mat image = cv::imread(qsfileName.toStdString());
       QImage _img(image.data,image.cols,image.rows,QImage::Format_RGB888);
       QPixmap *buffer = new QPixmap();
        *buffer = QPixmap::fromImage(_img);   //이미지를 버퍼에 옮긴다.
         *buffer = buffer->scaled(_img.width(),_img.height()); //이미지 사이즈 조절
        ui->label1->setPixmap(QPixmap::fromImage(_img));
          ui->label1->resize(buffer->width(),buffer->height()); //QLabel의 크기를 이미지 사이즈에 맞추어 조절


}
