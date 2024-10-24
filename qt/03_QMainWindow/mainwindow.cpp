#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QMainWindow
    /*  主窗口程序类包含
     *      菜单栏 menu bar： 窗口最顶端；菜单栏只能有一个
     *      一个状态栏 status bar： 窗口最底下
     *      一个中心部件 central widget： 程序的主要内容显示区域
     *      多个锚接部件 dock widgets： 可移动的小窗口或是小工具栏，包裹在central widget外边
     *      多个工具栏 tool bars： 菜单栏下方，包裹在dock widgets外边;可以拖动，默认处于窗口的上方停靠
    */

    //! 可以看设置方法，如果是set的就是这个组件最多有一个，如果是add的就是这个组件可以有很多个

    //! 菜单栏 最多有一个
    QMenuBar * bar = menuBar();
    setMenuBar(bar); // 将创建的菜单栏设置到窗口中,如果不设置，将不会在窗口中显示
    // 为菜单栏增加菜单选项,并使用QMenu对象接收创建的菜单对象，目的是为了增加子菜单
    QMenu * fileMenu = bar->addMenu("file");
    QMenu * eidtMenu = bar->addMenu("edit");

    // 创建菜单项
    QAction * newAction = fileMenu->addAction("new");
    // 为菜单中不同类的操作添加分割线
    fileMenu->addSeparator();
    QAction * openAction = fileMenu->addAction("open");

    //! 工具栏 可以有多个
    QToolBar * toolBar = new QToolBar(this); // 设置this加入对象树，简化内存回收操作
    // addToolBar(toolBar);
    addToolBar(Qt::RightToolBarArea, toolBar); // 可以设置toolbar默认的停靠位置
    /*Qt中所有的枚举值都是Qt::开头的
       ToolBar位置的枚举值
        Qt::LeftToolBarArea   0x1
        Qt::RightToolBarArea   0x2
        Qt::TopToolBarArea   0x4
        Qt::BottomToolBarArea   0x8
        Qt::AllToolBarAreas   ToolBarArea_Mask
        Qt::NoToolBarArea   0
    */

    // 可以设置允许工具栏停靠的位置 使用管道符增加可停靠的位置
    toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::BottomToolBarArea);

    // 设置是否允许浮动，即不停靠在窗口周围
    toolBar->setFloatable(false);

    // 设置是否允许移动
    toolBar->setMovable(false);

    // 在工具栏中设置内容
    toolBar->addAction(newAction);
    // 不同类的工具之间可以增加分割线
    toolBar->addSeparator();
    toolBar->addAction(openAction);

    // 工具栏中增加控件
    QPushButton * btn = new QPushButton("btn", this);
    toolBar->addWidget(btn);

    //! 状态栏 最多有一个
    QStatusBar * staBar =  statusBar();
    setStatusBar(staBar);

    // 放置一些标签控件
    QLabel * label = new QLabel("提示信息", this); // parent参数给this，加入对象树中，简化内存回收
    staBar->addWidget(label);

    QLabel * label2 = new QLabel("右侧提示信息", this);
    staBar->addPermanentWidget(label2);

    //! 锚接部件，浮动窗口 可以有多个
    QDockWidget * dockWidget = new QDockWidget("浮动", this);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    // 设置停靠的区域
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);

    //! 中心部件 只能有一个
    QTextEdit * edit = new QTextEdit(this);
    setCentralWidget(edit);

}

MainWindow::~MainWindow()
{
    delete ui;
}
