#include "mypushbutton.h"
#include <QDebug> // 引入调试,但是其输出的时候，是小写的qDebug

// 构造
MyPushButton::MyPushButton(QWidget *parent)
    : QPushButton{parent}
{
    // 可以理解为cout，会自动换行
    qDebug() << "MyPushButton 构造函数调用";
}

// 析构
MyPushButton::~MyPushButton()
{
    qDebug() << "MyPushButton 析构函数调用 ";
}
