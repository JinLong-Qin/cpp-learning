#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // 在使用ui文件设置了界面后，这一行会将绘制的页面添加到this中
    // 通过ui就可以找到之前在design中设计的组件
    // 但是这样的路径不方便
    // ui->actionnew->setIcon(QIcon("C:/Users/47551/Desktop/_cpp/qt/04_QtSource/resources/new.png"));

    // 首先将要用到的资源放到项目文件目录下
    // 新建resource文件，右键add new-> qt->resouce file
    // 在创建的文件中分别设置前缀和文件
    //! 使用资源方式 语法： ":前缀名/文件名"
    ui->actionnew->setIcon(QIcon(":/resources/Images/new.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
