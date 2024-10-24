#include "student.h"
#include <QDebug>

Student::Student(QObject *parent)
    : QObject{parent}
{}

void Student::treat()
{
    qDebug() << "请客吃饭";
}

void Student::treat(QString foodName){
    // 多出的空格是重载的<<运算符弄出来的，每一个<<连接的部分都会中间存在一个空格
    // QString -> char *要先转为QByteArray(使用.toUtf8())，然后在转为char* (使用data())
    qDebug() << "请老师吃" << foodName.toUtf8().data();
}
