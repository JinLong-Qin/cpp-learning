#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 读取文件，文件标题在lineedit，文件内容显示在textedit
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        QString path = QFileDialog::getOpenFileName(this, "打开文件", "C:\\Users\\47551\\Desktop\\_cpp\\qt\\011_QPaintDevices");
        ui->lineEdit->setText(path);

        //! 读文件
        // QFile 默认支持UTF-8格式
        QFile file(path);
        file.open(QIODevice::ReadOnly); // 设置文件只读

        QByteArray array = file.readAll();

        // QByteArray arr;
        // while(!file.atEnd()){
        //     arr += file.readLine(); // 按行读
        // }
        // file.close();
        ui->textEdit->setText(array);

        //! 写操作
        file.open(QIODevice::Append); // 以追加方式写  如果是覆盖写就是
        file.write("123213");
        file.close();

        //! QFileInfo 读取文件信息
        QFileInfo info(path);

        qDebug() << info.size() << info.suffix() << info.fileName() << info.filePath() << info.lastModified();
        // 使用QDateTime的toString函数格式化输出时间信息
        qDebug() << info.birthTime().toString("yyyy-MM-dd hh:mm:ss");
    });
}

Widget::~Widget()
{
    delete ui;
}
