#include "mywidget.h"
#include <QPushButton>
#include <mypushbutton.h>
#include <QDebug>

// 命名规范
// 类名，大驼峰
// 函数名，小驼峰

// 快捷键
// 注释 ctrl /
// 运行 ctrl r
// 编译 ctrl b
// 字体缩放 ctrl 鼠标滚轮
// 整行移动 ctrl shift 上箭头/下箭头
// 帮助文档 F1 选中要看文档的内容，然后按F1； 左边菜单栏的Help按钮
// 自动对齐 ctrl i
// 同名之间的 .h文件和.cpp文件 切换 F4

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent) // 初始化列表，表示，如果给定了parent，就用parent做初始化，如果没给，就用头文件中给的默认参数做初始化
{
    // 创建一个按钮
    QPushButton * btn = new QPushButton;
    // btn->show(); // show方法以顶层方式弹出窗口空间，即单独有一个窗口显示，所以让btn显示在现有窗口中，就不可以使用show方法
    btn->setParent(this); // 设置btn对象依赖在MyWidget窗口中  this指当前窗口

    // 按钮上文本设置
    btn->setText("第一个按钮");

    // 可以在实例化按钮的时候，就给定按钮上的字和其依赖的窗口
    QPushButton * btn2 = new QPushButton("第二个按钮", this);
    // 但是会出现1. 这种按钮的实例化方法会影响到窗口本身的大小，导致显示出来的窗口只有按钮大小
    // 2.同时，如果有两个按钮，后面实例化的会盖住前一个，

    // 使用move函数将btn2移动位置,解决2问题
    btn2->move(300, 200);

    // 使用resize函数去改变窗口大小，可以解决1问题
    resize(600, 400);

    // 设置固定窗口大小. 如果不想要让窗口被随意放大缩小，可以使用setfixedsize方法设置固定的窗口大小
    setFixedSize(600, 400);

    // 设置窗口标题
    setWindowTitle("第一个窗口");




    /*
     * ！[一定程度上简化了内存回收机制]
     * QT中存在[对象树]的概念，所有的控件创建在堆区（使用new关键字实例化）,其只要最终都继承自QObject（包括继承，或者是子类派生下来的），那么其创建的时候就会被加入到children列表中
     * 当父对象进行析构的时候，这个列表中的所有子对象也都会被析构
     * 类比当前这个按钮的案例就是：btn是QPushButton，设置了其挂在的父对象是MyWidget，
     * MyWidget继承自QWidget，
     * 最后QWidget继承自QObject
     *
     * 当按钮对象进行实例化的时候，构造函数执行顺序由父类到子类
     * 其析构过程相反，从子类到父类
     *      值得注意的是，父类的析构代码会先执行，但是执行后不会先释放父类对象，而是会遍历去寻找当前父类是否存在子类， 然后将子类的析构代码执行完，再去遍历子类是否还存在子类，当找不到子类的时候再回头把对象一个个释放掉
     *
     * 因此，new出来的对象不用手动delete
     *
    */


    // 创建一个自己的按钮对象
    MyPushButton * myBtn = new MyPushButton;
    myBtn->setText("自定义按钮类");
    myBtn->move(0, 200);
    // qt窗口页面坐标轴以左上角为原点(0,0)
    // x坐标轴正方向向右，y坐标轴正方向是向下

    myBtn->setParent(this);


    /*  现在我们得到了一个带有三个按钮的页面，但是按钮点击之后并没有任何作用
     *  要为按钮增加操作功能，点击按钮（发出信号），执行操作（信号的处理；也叫做槽）
     *  [信号和槽]
     *  信号槽的优点，松散耦合， 信号的发送端和接收端本身没有关联，通过使用connect函数连接，将两者 耦合在一起。
     *
     *  connect(信号的发送者，发送的信号[一定要是函数的地址], 函数的接收端, 处理的槽函数[一定要是函数的地址])
     *  信号：
     *      clicked() 点击
     *      pressed() 按下
     *      released() 松开
     *      toggled() 切换
    */
    // 为自定义按钮增加点击后关闭窗口的功能
    // connect(myBtn, &MyPushButton::clicked, this, &MyWidget::close);
    connect(myBtn, &QPushButton::clicked, this, &QWidget::close); // 也可以使用父类去实现这个功能

}

MyWidget::~MyWidget() {
    qDebug() << "MyWidget 析构调用";
}
