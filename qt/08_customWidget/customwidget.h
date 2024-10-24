#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

namespace Ui {
class CustomWidget;
}

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = nullptr);
    void setNum(int num);
    int getNum();
    ~CustomWidget();

private:
    Ui::CustomWidget *ui;
};

#endif // CUSTOMWIDGET_H
