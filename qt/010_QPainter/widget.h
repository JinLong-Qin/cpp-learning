#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>

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

    // 重写绘画事件
    void paintEvent(QPaintEvent *event);

    int posY=0; // 用于调整绘制位图的起始x位置 实现手动调用绘图事件的功能

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
