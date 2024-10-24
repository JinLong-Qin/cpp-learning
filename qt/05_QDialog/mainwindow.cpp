#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 对话框

    // 点击按钮，弹出对话框
    connect(ui->actionnew, &QAction::triggered, [=](){
        //! 对话框
        //! 1. 模态对话框（对话框出现后，不可以对其他的窗口进行操作）
        QDialog dlg(this); // 不是new的，所以在栈上
        dlg.resize(200, 100);
        dlg.exec(); // 以模态的方式,会阻塞后面的代码
        qDebug() << "模态对话框弹出";
        //! 2. 非模态对话框（对话框出现后，可以对其他的窗口进行操作）

        // QDialog dlg2(this); // 栈上的对象会在局部函数执行完之后自动都回收，所以将dlg2放置在堆上
        QDialog * dlg2 = new QDialog(this);
        dlg2->resize(200, 100);
        dlg2->setAttribute(Qt::WA_DeleteOnClose); // 设置55号属性，表示当检测到关闭操作的时候，删除这个对象。其他属性见QWidget.setAttribute中详细内容
        dlg2->show();
        qDebug() << "非模态对话框弹出";

        //! 消息对话框
        //! 参数列表[父亲，标题，显示内容，按键类型，默认关联回车的按键(即默认按键，点击回车会执行的)]
        //! QMessageBox静态成员函数，所以可以直接使用类名::方法名调用方法
        /*  第四个参数是按钮，可以将对话框按钮进行改变  枚举值
         *  QMessageBox::Ok                 0x00000400An "OK" button defined with the AcceptRole.
            QMessageBox::Open               0x00002000An "Open" button defined with the AcceptRole.
            QMessageBox::Save               0x00000800A "Save" button defined with the AcceptRole.
            QMessageBox::Cancel             0x00400000A "Cancel" button defined with the RejectRole.
            QMessageBox::Close              0x00200000A "Close" button defined with the RejectRole.
            QMessageBox::Discard            0x00800000A "Discard" or "Don't Save" button, depending on the platform, defined with the DestructiveRole.
            QMessageBox::Apply              0x02000000An "Apply" button defined with the ApplyRole.
            QMessageBox::Reset              0x04000000A "Reset" button defined with the ResetRole.
            QMessageBox::RestoreDefaults    0x08000000A "Restore Defaults" button defined with the ResetRole.
            QMessageBox::Help               0x01000000A "Help" button defined with the HelpRole.
            QMessageBox::SaveAll            0x00001000A "Save All" button defined with the AcceptRole.
            QMessageBox::Yes                0x00004000A "Yes" button defined with the YesRole.
            QMessageBox::YesToAll           0x00008000A "Yes to All" button defined with the YesRole.
            QMessageBox::No                 0x00010000A "No" button defined with the NoRole.
            QMessageBox::NoToAll            0x00020000A "No to All" button defined with the NoRole.
            QMessageBox::Abort              0x00040000An "Abort" button defined with the RejectRole.
            QMessageBox::Retry              0x00080000A "Retry" button defined with the AcceptRole.
            QMessageBox::Ignore             0x00100000An "Ignore" button defined with the AcceptRole.
            QMessageBox::NoButton
         *
         *
        */
        //! 错误对话框是一个模态对话框，因为在弹出消息对话框时，不允许操作窗口内其他内容
        QMessageBox::critical(this, "critical", "错误");

        //! 信息对话框
        QMessageBox::information(this, "information", "信息");

        //! 问题对话框
        QMessageBox::question(this, "question", "提问"); // 会有两个按钮，默认是确定和取消
        // 对第四个参数进行修改，使其显示的按钮变化 多个按钮用管道符做分割
        QMessageBox::question(this, "question2", "保存", QMessageBox::Save|QMessageBox::Cancel);
        // 第五个参数是按回车默认选择的按钮是哪一个
        QMessageBox::question(this, "question2", "保存", QMessageBox::Save|QMessageBox::Cancel, QMessageBox::Cancel); // 设置按回车选中的是Cancel按钮

        //! 如何确认用户点击的是哪一个按键 这些对话框返回值都是其点击按钮的枚举值,使用对应按键的枚举值和其返回值做判断
        if (QMessageBox::Save == QMessageBox::question(this, "question2", "保存", QMessageBox::Save|QMessageBox::Cancel, QMessageBox::Cancel)){
            qDebug() << "点击了保存";
        }else{
            qDebug() << "点击了取消";
        }

        //! 警告对话框
        QMessageBox::warning(this, "waring", "警告");


        //! 其他标准对话框
        //! 颜色的对话框，参数为打开对话框之后默认选中的颜色
        QColor color = QColorDialog::getColor(QColor(255, 0, 0));
        qDebug() << color.red() << color.green() << color.blue() << color.alpha();

        //! 文件对话框,[父亲，标题，默认打开文件路径(不给这个参数，默认打开一个当前目录下的)，文件过滤器(只会过滤文件，会保留所有的文件夹)]
        //! 返回值是选择的文件的路径，类型为QString
        QString str = QFileDialog::getOpenFileName(this, "open", "C:\\Users\\47551\\Desktop", "(*.zip)");
        qDebug() << str;

        //! 字体对话框
        bool flag;
        QFont font = QFontDialog::getFont(&flag, QFont("华文彩云", 36)); // 设置默认华文彩云36号字
        qDebug() << font.family().toUtf8().data() << font.pointSize() << font.bold() << font.italic();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
