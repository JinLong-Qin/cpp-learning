#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);

    // 槽函数要求
    // 1. 返回值void 需要声明也需要实现
    // 2. 可以有参数，可以发生重载
    void treat();

    void treat(QString foodName);

signals:

// 早期的QT版本，槽函数必须要卸载public slots中，高级版本可以写在全局或者public下
// public slots:

};

#endif // STUDENT_H
