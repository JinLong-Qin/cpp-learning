#include "mywidget.h" // 头文件

#include <QApplication> // 包含一个应用程序类的头文件

// 程序入口
// argc命令行变量的数量，argv命令行变量的数组
int main(int argc, char *argv[])
{
    // 应用程序对象，在QT中，应用程序对象有且只有一个
    QApplication a(argc, argv);
    // 窗口对象 mywidget的父类-> QWidget
    MyWidget w;
    // 窗口对象默认不会显示，要调用show方法，才会显示窗口
    w.show();

    // 让应用程序对象进入 [消息循环机制]
    // 让代码阻塞到这里，直到点击窗口的关闭按钮
    return a.exec();

    // 另有.pro项目文件的内容解释，见.pro文件
}
