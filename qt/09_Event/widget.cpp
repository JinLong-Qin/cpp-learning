#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 类实现定时器
    // startTimer(定时器间隔[单位毫秒])
    // startTimer返回值是定时器的唯一标识
    timerId1 = startTimer(1000);
    timerId2 = startTimer(2000);

    // 2.QTimer 实现定时器
    // 创建定时器对象
    QTimer * timer = new QTimer(this);
    timer->start(500);
    static int num=1;
    connect(timer, &QTimer::timeout, [=](){ // timeout指每次到了间隔都会发出信号
        // static int num=1;
        ui->label_4->setText(QString::number(num++));
    });

    // 绑定事件，点击按钮，计时器停止
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        if(ui->pushButton->text() == "stop"){
            timer->stop();
            ui->pushButton->setText(QString("continue"));
        }else if(ui->pushButton->text() == "continue"){
            timer->start(500);
            ui->pushButton->setText(QString("stop"));
        }else if(ui->pushButton->text() == "start"){
            timer->start(500);
            ui->pushButton->setText(QString("stop"));
        }
    });

    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        timer->stop();
        num=1;
        ui->label_4->setText(QString::number(0));
        ui->pushButton->setText(QString("start"));
    });

    // 为label安装事件过滤器
    // 步骤1，谁要安装过滤器，就让谁的父类给做安装
    ui->label->installEventFilter(this);
    // 步骤2，重写 eventFiliter 事件
}
// 步骤2，重写 eventFiliter 事件
bool Widget::eventFilter(QObject *watched, QEvent *event){
    if(watched == ui->label){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent * ev = dynamic_cast<QMouseEvent *>(event);
            qDebug() << "事件过滤器中，鼠标按下" << ev->pos();
            return true; // true表明用户要自己处理这个事件，不继续向下分发
        }
    }
    // 其他事件，还做默认处理
    return QWidget::eventFilter(watched, event);
} // 事件过滤器拦截的事件，不会进入事件分发器了，
// 事件过滤器的位置在，程序将事件分发到事件分发器之前

Widget::~Widget()
{
    delete ui;
}


// 1. 使用事件实现定时器
void Widget::timerEvent(QTimerEvent *e) {
    // 通过QTimerEvent中的timerId来判断到底但是调用的哪一个定时器
    if(e->timerId() == timerId1){
        static int num=1; // 使用static关键字延长num的生命周期
        // label_2每一秒+1
        ui->label_2->setText(QString::number(num++));
    }

    if(e->timerId() == timerId2){
        static int num=1; // 使用static关键字延长num的生命周期
        // label_2每一秒+1
        ui->label_3->setText(QString::number(num++));
    }

}


