#include "widget.h"
#include "ui_widget.h"
#include <QRadioButton>
#include <QDebug>
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>
#include <QTimeZone>
#include <QMovie>
#include <QMenuBar>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //! 菜单栏
    QMenuBar * menu = new QMenuBar(this);
    // setMenuBar(menu);
    QMenu * fileMenu = menu->addMenu("文件");
    QMenu * editMenu = menu->addMenu("编辑");

    QAction * newAction = fileMenu->addAction("新建");
    fileMenu->addSeparator();
    QAction * openAction = fileMenu->addAction("打开");

    connect(newAction, &QAction::triggered, [=](){ // triggered函数提供用户触发的信号
        qDebug() << "点击 文件-新建";
    });

    //! toolButton 可以通过setToolButtonStyle设置toolbutton的样式，可以设置显示图片
    /*ToolButtonStyle的枚举值
     * ConstantValueDescription
        Qt::ToolButtonIconOnly          0   Only display the icon.
        Qt::ToolButtonTextOnly          1   Only display the text.
        Qt::ToolButtonTextBesideIcon    2   The text appears beside the icon.
        Qt::ToolButtonTextUnderIcon     3   The text appears under the icon.
        Qt::ToolButtonFollowStyle       4   Follow the style.

     *
     *
    */
    // ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // ui->toolButton->setIcon() // 设置图标

    //! 单选框 radioButton
    // 设置单选框默认选中
    ui->rbtnMan->setChecked(true);

    // 设置监听事件，点击女后打印
    connect(ui->rbtnWomen, &QRadioButton::clicked, [=](){

        qDebug() << "选中女生";
    });

    ui->checkBox_3->setTristate();// 为复选框设置三个状态，多一个待选，待选的状态是1

    connect(ui->checkBox_3, &QCheckBox::stateChanged, [=](int state){
        // 信号的参数要大于等于槽函数的参数，并且类型要一致，信号参数自动能传递给槽函数
        // 所以这里stateChanged有一个int类型的参数，将其直接传递给槽函数，所以在小括号中也给一个int类型的参数
        qDebug() << state;
        //! checkbox的值，始终在(2, 0)之间切换，2是选中，0是未选中，如果设置了待选状态，待选状态的状态码是1

    });

    //!  列表容器 ListWidget
    // 给listwidget组件添加内容
    QListWidgetItem * qitem = new QListWidgetItem("锄禾日当午");
    /*对齐方式的枚举值
     * The horizontal flags are:
        ConstantValueDescription
        Qt::AlignLeft           0x0001Aligns with the left edge.
        Qt::AlignRight          0x0002Aligns with the right edge.
        Qt::AlignHCenter        0x0004Centers horizontally in the available space.
        Qt::AlignJustify        0x0008Justifies the text in the available space.

       The vertical flags are:
        ConstantValueDescription
        Qt::AlignTop            0x0020Aligns with the top.
        Qt::AlignBottom         0x0040Aligns with the bottom.
        Qt::AlignVCenter        0x0080Centers vertically in the available space.
        Qt::AlignBaseline       0x0100Aligns with the baseline.

       You can use only one of the horizontal flags at a time. There is one two-dimensional flag:
        ConstantValueDescription
        Qt::AlignCenter         AlignVCenter | AlignHCenterCenters in both dimensions


     *
    */
    // 设置对齐方式
    qitem->setTextAlignment(Qt::AlignHCenter);
    // 添加item到list中
    ui->listWidget->addItem(qitem);

    // 批量添加item
    QStringList list;
    list << "锄禾日当午" << "汗滴禾下土" << "谁知盘中餐" << "粒粒皆辛苦"; // 重载了<<运算符，可以直接使用<<来将item拼接到list中
    ui->listWidget->addItems(list);
    // 这样的局限性是没办法单独设置对齐方式，如果要设置对齐方式，还是要使用addItem的方式


    //! treewidget使用
    // 设置水平头
    ui->treeWidget->setHeaderLabels(QStringList() << "英雄" << "英雄介绍");

    QTreeWidgetItem * liItem = new QTreeWidgetItem(QStringList() << "力量");
    QTreeWidgetItem * minItem = new QTreeWidgetItem(QStringList() << "敏捷");
    QTreeWidgetItem * zhiItem = new QTreeWidgetItem(QStringList() << "智力");
    // 增加根节点
    ui->treeWidget->addTopLevelItem(liItem);
    ui->treeWidget->addTopLevelItem(minItem);
    ui->treeWidget->addTopLevelItem(zhiItem);

    // 追加子节点
    QTreeWidgetItem * l1 = new QTreeWidgetItem(QStringList() << "钢背兽" << "法坦");
    liItem->addChild(l1); // 使用addchild将子节点挂在在根节点上
    QTreeWidgetItem * l11 = new QTreeWidgetItem(QStringList() << "钢背兽" << "法坦");
    l1->addChild(l11); // 使用addchild将子节点挂在在l1上


    //! tableWidget
    // 要先设置列
    ui->tableWidget->setColumnCount(3);
    // 才能设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "姓名" << "性别" << "年龄");

    // 设置行数
    ui->tableWidget->setRowCount(5);

    // 设置表格正文
    // ui->tableWidget->setItem(0, 0, new QTableWidgetItem("亚瑟"));
    // 批量设置
    // 姓名
    QStringList nameList;
    nameList << "亚瑟" << "赵云";
    // 性别
    QList<QString> genderList; // 另一种方式
    genderList << "男" << "男";

    for (int i = 0; i < nameList.size(); ++i) {
        int col=0;
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(nameList[i]));
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(genderList.at(i))); // at的方式越界会抛出异常
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(QString::number(rand()%20)));
        // int 转 QString

    }

    //! 通过在创建组件时设置其父亲为对应位置的组件的id号，即可在组件中增加新的子组件
    //! 栈控件
    // 栈控件通过CurrentIndex来标记当前子控件的id，通过设置CurrentIndex就可以做到切换页面
    connect(ui->btn_scoll, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btn_tool, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 手动添加控件到栈控件中
    ui->stackedWidget->addWidget(ui->tabWidget);
    ui->stackedWidget->setCurrentIndex(2);

    // ui->stackedWidget->widget(0); // 通过widget(id)来获取对应位置的组件

    //! tabwidget
    QWidget * newTab = new QWidget(this);
    QPushButton * btn_tabWidget = new QPushButton("123", newTab);

    ui->tabWidget->addTab(newTab, QString("搜狐"));

    // ui->tabWidget->widget(2).add

    //ui->tabWidget->removeTab(0);//移除id为0的选项卡
    //ui->tabWidget->clear();//清空选项卡

    //! toolbox
    ui->toolBox->setCurrentIndex(1);
    QPushButton * btnToolBOX1 = new QPushButton(ui->toolBox->widget(1));
    btnToolBOX1->setText("in btnToolBOX1");

    //! combobox
    ui->comboBox->addItem("宝马");
    ui->comboBox->addItem("奔驰");

    // 给按钮pushButton_11添加监听，点击按钮后，直接选择奔驰
    connect(ui->pushButton_11, &QPushButton::clicked, [=](){
        ui->comboBox->setCurrentIndex(1); // 可以使用setCurrentIndex来直接为comboBox设置选中值
        // ui->comboBox->setCurrentText("拖拉机"); // 也可以使用setCurrentText来设置选中，但是，参数中的值一定要存在于下拉框选项中，否则就不会起作用
        // 获取当前comboBox的值，并打印
        qDebug() << ui->comboBox->currentText();

    });

    //! lineEdit
    /*
     * ConstantValueDescription
        QLineEdit::Normal               0   Display characters as they are entered. This is the default.
        QLineEdit::NoEcho               1   Do not display anything. This may be appropriate for passwords where even the length of the password should be kept secret.
        QLineEdit::Password             2   Display platform-dependent password mask characters instead of the characters actually entered.
        QLineEdit::PasswordEchoOnEdit   3   Display characters as they are entered while editing otherwise display characters as with Password.

     *
    */
    ui->lineEdit->setEchoMode(QLineEdit::Password); // 可以通过setEchoMode来设置输入框的显示模式 password就是掩码模式
    connect(ui->lineEdit, &QLineEdit::editingFinished, [=](){
        qDebug() << ui->lineEdit->displayText(); // 当echomode是password的时候，获取的也是掩码，得到的是显示在输入框中的样子
        qDebug() << ui->lineEdit->text();// 使用text获取输入内容
    });

    //! TextEdit控件和PlainEdit的区别是，TextEdit可以支持文本样式，而PlainEdit只能显示普通的文字


    //! spinBox
    ui->spinBox->setMaximum(20);
    ui->spinBox->setMinimum(0);
    qDebug() << ui->spinBox->maximum() << ui->spinBox->minimum();
    connect(ui->spinBox, &QSpinBox::textChanged, [=](){
        qDebug() << ui->spinBox->text(); // 返回QString类型
        qDebug() << ui->spinBox->value(); // 返回int类型
        // 通过修改spinbox的值，连带着水平滑动条变化
        ui->horizontalScrollBar->setValue(ui->spinBox->value());
        // 通过修改spinbox的值，连带着垂直滑动条变化
        ui->verticalSlider->setValue(ui->spinBox->value());
    });

    //! timeEdit
    ui->timeEdit->setTime(QTime(16,0,0)); // 设置时间 16时 0分 0秒
    connect(ui->timeEdit, &QTimeEdit::userTimeChanged, [=](){
        qDebug() << ui->timeEdit->time();
    });

    //! dateEdit
    ui->dateEdit->setDate(QDate(2024, 9, 10)); // 设置时间为2024年 9月 10日
    connect(ui->dateEdit, &QDateEdit::userDateChanged, [=](){
        qDebug() << ui->dateEdit->date();
    });

    //! datetimeEdit
    ui->dateTimeEdit->setDateTime(QDateTime(QDate(2024, 9, 10), QTime(16,0,0))); // 设置时间为 2024年 9月 10日 16时 0分 0秒 ；可以设置时区，东八区（QTimeZone("Asia/Shanghai")）

    //! horizontalScrollBar
    ui->horizontalScrollBar->setMaximum(ui->spinBox->maximum());
    ui->horizontalScrollBar->setMinimum(ui->spinBox->minimum());
    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, [=](int value){
        qDebug() << value;
        // 通过水平滑动条变化，连带着修改spinbox的值
        ui->spinBox->setValue(value);
    });
    //! verticalSlider
    ui->verticalSlider->setMaximum(ui->spinBox->maximum());
    ui->verticalSlider->setMinimum(ui->spinBox->minimum());
    connect(ui->verticalSlider, &QScrollBar::valueChanged, [=](int value){
        qDebug() << value;
        // 联动修改，将spinbox的值也做修改
        ui->spinBox->setValue(value);
    });

    //! label不光可以显示文字，还可以是图片
    ui->label->setPixmap(QPixmap(":/Images/new.png"));
    ui->label->setScaledContents(true); // 设置自动处理内容大小，自动缩放

    // 还可以显示动图
    QMovie * movie = new QMovie(":/Images/loading.gif");
    ui->label_2->setMovie(movie);
    ui->label_2->setScaledContents(true); // 设置自动处理内容大小，自动缩放
    // 播放动图
    movie->start();

    //! progressBar 和 doublespinbox的联动
    ui->progressBar->setValue(ui->doubleSpinBox->value()); // 初始化设置两者表示相同
    connect(ui->doubleSpinBox, &QDoubleSpinBox::valueChanged, [=](int value){ // 根据doublespinbox的修改，调整progressbar的值
        ui->progressBar->setValue(value);
    });
    // connect(ui->progressBar, &QProgressBar::valueChanged, [=](int value){
    //     ui->doubleSpinBox->setValue(value);
    // });

}

Widget::~Widget()
{
    delete ui;
}
