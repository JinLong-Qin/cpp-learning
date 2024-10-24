#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 点击按钮，获取当前的值
    connect(ui->btn_get, &QPushButton::clicked, [=](){
        qDebug() << ui->widget->getNum();
    });

    // 点击按钮，设置水平滑块到一半
    connect(ui->btn_set, &QPushButton::clicked, [=](){
        ui->widget->setNum(50);
    });
}

Widget::~Widget()
{
    delete ui;
}
