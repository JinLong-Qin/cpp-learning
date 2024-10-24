#ifndef WIDGET_H
#define WIDGET_H

// 引入学生老师头文件
#include "student.h"
#include "teacher.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    Teacher * tea;
    Student * stu;

    // 增加触发器，调用tea的hungry函数发出信号
    void classIsOver();
};
#endif // WIDGET_H
