#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    // 构造函数
    explicit MyPushButton(QWidget *parent = nullptr);

    // 查看构造和析构
    ~MyPushButton();

signals:
};

#endif // MYPUSHBUTTON_H
