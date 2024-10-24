#ifndef MYWIDGET_H // 防止重复包含头文件，开始
#define MYWIDGET_H

#include <QWidget> // 包含头文件 QWidget 窗口类

class MyWidget : public QWidget // 继承
{
    Q_OBJECT // 宏，允许类中使用[信号和槽]的机制

public:
    MyWidget(QWidget *parent = nullptr); // 有参构造函数 参数给默认值，声明和实现只能有一个地方有默认参数
    ~MyWidget();
};
#endif // MYWIDGET_H // 防止重复包含头文件，开始
