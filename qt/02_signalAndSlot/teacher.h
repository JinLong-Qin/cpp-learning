#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);

signals:
    // 自定义信号，写在这里
    // 要求：
    // 1.返回值是void，并且只需要声明，不需要实现
    // 2. 可以有参数，即可以进行重载
    void hungry();

    // hungry信号的重载，老师给出信号，说明自己要吃啥
    void hungry(QString foodName);
};

#endif // TEACHER_H
