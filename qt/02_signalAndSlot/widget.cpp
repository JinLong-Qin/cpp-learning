#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>

// 实现自定义信号和槽
// teacher 类
// student 类

// 下课后,老师发出信号，饿了，学生接收信号，请吃饭

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 老师对象
    this->tea = new Teacher(this);
    // 学生对象
    this->stu = new Student(this);

    // 为teacher的hunger添加触发器，当hungry函数触发，就会发出信号，执行student的treat函数
    // 具体到本案例中，触发条件为下课了，当下课，老师就饿[发出信号]，学生做反应请吃饭[槽函数]
    // connect(tea, &Teacher::hungry, stu, &Student::treat);

    // 下课函数
    // 触发老师饿了的信号
    // classIsOver();

    //! 当自定义的信号和槽函数出现了重载
    //! 需要利用函数指针，明确指出函数地址

    // 使用函数指针明确信号和槽，
    void (Teacher::*teacherSignale) (QString) = &Teacher::hungry;
    void (Student::*studentSlot)(QString) = &Student::treat;
    connect(tea, teacherSignale, stu, studentSlot);

    // classIsOver();

    // 但是发现，打印结果最后是： 请老师吃 "宫保鸡丁"  传入的参数被双引号括住了
    // 这是由于参数是QString类型，而在studeng.cpp的treat方法实现时，这个参数前面拼接的部分是char *类型的，因此要想办法将传入的QString类型的参数转为char *
    // 见student.cpp的treat方法

    // 构建按钮，当点击按钮后触发下课，发送信号，
    QPushButton * btn = new QPushButton("下课", this);

    // 重设窗口大小
    resize(600, 400);

    // 点击按钮，触发下课
    connect(btn, &QPushButton::clicked, this, &Widget::classIsOver);

    // 无参信号槽 使用void表示无参
    void (Teacher::*teacherSignale2) (void) = &Teacher::hungry;
    void (Student::*studentSlot2)(void) = &Student::treat;
    connect(tea, teacherSignale2, stu, studentSlot2);

    // 使用信号连接信号，
    // 将按钮的点击信号，连接到了老师的饿了信号，饿了信号在56行又连接了学生的请吃饭的槽函数
    connect(btn, &QPushButton::clicked, tea, teacherSignale2);

    // 断开连接
    // 使用disconnect断开信号之间的连接，和connect中参数一致，可以将建立连接的时候的参数完全复制
    // disconnect(tea, teacherSignale2, stu, studentSlot2);


    /*拓展
     *  1.信号可以连接信号
     *  2.一个信号可以连接多个槽函数
     *  3.多个信号可以连接同一个槽 函数
     *  4.信号和槽函数的参数数据类型，必须一一对应，
     *  5.信号的参数可以比槽函数的参数数量多，但是对应位置的数据类型必须要相同
     *      如54行的无参信号槽例子，如果使用了有参就不行，原因是因为60行的时候，click信号的参数是bool类型，teacherSignale2槽函数如果是无参的，1个参数大于0个参数所以可以
     *      但是如果，使用了34行的有参例子，那click信号的参数是bool类型，teacherSignale参数是QString，虽然1个参数等于1个参数，但是对应位置的参数数据类型不同，所以报错
    */

    // // qt4之前的信号槽 连接无参
    // connect(tea, SIGNAL(hungry()), stu, SLOT(treat()));
    // // 优点，参数直观，要是使用有参版本的，直接将参数类型放进去hungry(QString)
    // // 缺点，参数类型不做检测，比如下面槽函数的参数对于信号的参数，在运行的时候才会报错
    // connect(tea, SIGNAL(hungry()), stu, SLOT(treat(QString)));



    // Lambda表达式，用于定义并创建匿名函数对象
    /*
    语法：[函数对象参数](操作符重载函数参数) mutable -> 返回值(函数体)
        []部分不可以省略，这标志着一个Lambda表达式的开始
            空：表示没有使用任何函数对象参数
            =： 表示函数体内可以使用Lambda所在作用范围内所有可见的局部变量，包括lambda所在类的this，使用值传递方式
            &： 表示函数体内可以使用Lambda所在作用范围内所有可见的局部变量，包括Lambda所在类的this，使用引用传递方式
            this: 函数体内可以使用Lambda所在类中的成员变量
            a：将a按值传递方式进行传递，默认传递带有const关键字，如果要修改传递进来的a的拷贝，要添加mutable修饰符，要注意不会修改原值
            &a：将a按引用方式传递
            a, &b：a按照值传递，b按照引用传递
            =, &a, &b：除了a和b按照引用传递，其他参数都按照值传递
            &, a, b：除了a和b按照值传递，其他参数都按照引用传递
        要注意的是，lambda是定义匿名函数，函数执行要加小括号做调用。
    */
    // [btn](){btn->setText("aaa");}; // 构建匿名函数，将btn传递进去，使用btn将按钮文本进行设置为aa，但是并没有效果，原因是因为定义了匿名函数并没有执行
    // QPushButton * btn2 = new QPushButton;
    [btn](){
        btn->setText("aaa");
        // btn2->setText("aaa"); // 无法修改，因为btn2没有传递进来
    }();

    // lambda返回值, 使用->数据类型，标记返回值数据类型
    int ret = []()->int{return 100;}(); // 要记得在最后增加小括号表示函数执行
    qDebug() << "ret = " << ret;


    QPushButton * btn2 = new QPushButton;
    btn2->setText("lambda 绑定槽函数");
    btn2->move(200, 0);
    btn2->setParent(this);
    // 普通方式绑定点击信号关闭窗口，这样不灵活
    // connect(btn2, &QPushButton::clicked, this, &QWidget::close);
    // 使用lambda做绑定
    connect(btn2, &QPushButton::clicked, this, [=](){
        // 匿名函数的好处就是什么都可以写，可以在一个槽函数中有多个操作
        emit tea->hungry("宫保鸡丁");
        btn2->setText("123");
        // this->close();
    });


}

void Widget::classIsOver(){
    // emit tea->hungry(); // 使用emit关键字，触发hungry信号
    emit tea->hungry("宫保鸡丁"); // 有参传递
}

Widget::~Widget()
{
    delete ui;
}
