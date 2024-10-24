#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QEnterEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);

    // 重写鼠标进入事件
    void enterEvent(QEnterEvent *event);

    // 鼠标离开
    void leaveEvent(QEvent *event);

    // 重写这些函数
    virtual void mouseMoveEvent(QMouseEvent *ev) ;
    virtual void mousePressEvent(QMouseEvent *ev) ;
    virtual void mouseReleaseEvent(QMouseEvent *ev) ;
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    // 事件分发器
    // 通过event事件分发器 拦截 鼠标按下事件
    bool event(QEvent * e);

signals:
};

#endif // MYLABEL_H
