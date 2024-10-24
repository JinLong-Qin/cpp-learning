#include "mylabel.h"
#include <QDebug>

MyLabel::MyLabel(QWidget *parent)
    : QLabel{parent}
{
    //! 设置鼠标追踪
    // setMouseTracking(true); // 如果设置为true，就不需要鼠标点击，也可以触发鼠标移动事件
}

// 1. 查文档，找到对应的事件函数，对其重写
// 重写鼠标进入事件
void MyLabel::enterEvent(QEnterEvent *event){
    qDebug() << "触发 [鼠标进入事件]" ;
}

// 鼠标离开
void MyLabel::leaveEvent(QEvent *event){
    qDebug() << "触发 [鼠标离开事件]" ;
}

// 2. 将页面中的label控件，提升为MyLabel，
// 此时就可以直接触发事件了

// 鼠标移动
/*判断鼠标左右按键
 *  Qt::LeftButton  0x00000001  The left button is pressed, or an event refers to the left button. (The left button may be the right button on left-handed mice.)
    Qt::RightButton 0x00000002  The right button.
    Qt::MiddleButton    0x00000004  The middle button
 *
*/
//! 当只是想要知道，是哪一个按钮触发了事件的时候，用==就可以了
//! 当需要知道，是否是多个按钮同时按下时候，要用&符号
void MyLabel::mouseMoveEvent(QMouseEvent *ev) {
    qDebug() << "触发 [鼠标移动事件]" << ev->pos() << ev->globalPosition(); // globalPosition是从屏幕的左上角开始计算的
}

// 鼠标按下
void MyLabel::mousePressEvent(QMouseEvent *ev) {
    qDebug() << "触发 [鼠标按下事件]" << ev->pos();
    if(ev->button() == Qt::LeftButton){
        qDebug() << "\t鼠标左键" ;
    }else if(ev->button() == Qt::RightButton){
        qDebug() << "\t鼠标右键";
    }

}

// 鼠标释放
void MyLabel::mouseReleaseEvent(QMouseEvent *ev) {
    qDebug() << "触发 [鼠标释放事件]" ;
    if(ev->button() == Qt::LeftButton){
        qDebug() << "\t鼠标左键";
    }else if(ev->button() == Qt::RightButton){
        qDebug() << "\t鼠标右键";
    }
}

// 鼠标双击
void MyLabel::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "触发 [鼠标双击事件]" ;
    // event->buttons()
    if(event->buttons() & Qt::LeftButton){
        qDebug() << "\t鼠标左键";
    }else if(event->buttons() & Qt::RightButton){
        qDebug() << "\t鼠标右键";
    }
}

// 拦截鼠标按下事件
// qt中的事件并不是直接给对应的处理事件程序的，而是会通过一个中间件[事件分发器]来实现分发的功能
// 分发器函数原型 bool event(QEvent* e) 如果返回值是true，代表用户就要自己处理这个函数，就不会向下分发事件，否则就会由程序分发到对应的事件函数中
bool MyLabel::event(QEvent * e){
    /*
     *  QEvent::MouseButtonDblClick     4   Mouse press again (QMouseEvent).
        QEvent::MouseButtonPress        2   Mouse press (QMouseEvent).
        QEvent::MouseButtonRelease      3   Mouse release (QMouseEvent).
        QEvent::MouseMove               5   Mouse move (QMouseEvent)
     *
    */
    if(e->type() == QEvent::MouseButtonPress ){ // 如果是按下事件，就做拦截，不再分发到 MouseButtonPress 事件了
        QMouseEvent * ev = dynamic_cast<QMouseEvent *>(e);
        /*QEvent作为QMouseEvent的父类，并不存在pos方法。要想获得点击时的坐标，就要进行类型转换
         *  static_cast
         *      static_cast用于在基本类型之间进行转换，或者在具有隐式转换规则的情况下进行类型转换。
         *      static_cast主要适用于以下情况：
         *      1. 基本类型之间的转换：如从int到float，或者从char到int。
         *      2. 基本类型指针之间的转换：如从void*到char*
         *      3. 从派生类到基类的转换：static_cast可以将派生类的指针或引用转换为基类的指针或引用。
         *      4. (不建议)从基类到派生类的转换（向下转换）：但这种转换是有风险的，可以进行一定程度的向下转换，但是前提是程序员要知道转换是安全的。
         *          不安全的向下转换，如实际对象不是目标子类类型，访问子类特有的成员会导致崩溃或其他错误。
         *          因为在编译时就决定了类型，如果转换错误会导致[运行时错误]。
         *         在编译时进行类型转换，不依赖于RTTI 运行时类型信息，
         *     static_cast假定转换是合法的，不进行任何运行时检查。
         *
         *  dynamic_cast
         *      dynamic_cast主要用于处理多态性和运行时类型识别（RTTI 运行时类型信息）。
         *      它主要用于指针或引用的转换，特别是在[继承层次]中进行转换时。
         *      dynamic_cast在运行时检查转换的有效性（做检查了，所以保证了安全性）,依赖于（RTTI 运行时类型信息），
         *          如果转换成功，则返回转换后的指针或引用；
         *          如果失败，则返回nullptr（对于指针）或抛出std::bad_cast异常（对于引用）。
         *      优点：类型安全：dynamic_cast 确保了类型转换在运行时是安全的，减少了由于错误类型转换导致的程序崩溃和未定义行为的风险。
         *           多态支持：在使用多态（即虚函数）的上下文中，dynamic_cast 是唯一可以确保安全地进行向下转换的方法。
         *           易于维护和调试 ：使用 dynamic_cast 可以更容易地识别和修复类型转换错误，因为它在运行时提供明确的失败指示（返回 nullptr 或抛出异常）。
         *
         *
         *这里更适合使用dynamic_cast
        */
        qDebug() << "Event函数中，鼠标按下" << ev->pos();
        return true; // 返回true，表示用户自己做处理，不再向下分发
    }

    // 其他的事件交给父类处理
    return QLabel::event(e);
}

// 在程序和事件分发器中间，还可以使用一个[事件过滤器]，在程序分发到event之前再做一次高级拦截
// 使用步骤
// 1. 给控件 安装事件过滤器 installEventFiliter
// 2. 重写事件过滤函数 eventFiliter
// 见widget.cpp
