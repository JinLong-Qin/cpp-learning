#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimerEvent>

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

    // 重写定时器事件
    virtual void timerEvent(QTimerEvent *e) override;

    int timerId1;
    int timerId2;

    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
