#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QImage>
#include <QPaintEvent>
#include <QDebug>
#include <QPicture>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //! 绘图设备
    //! Pixmap绘图设备 专门为平台做了显示优化，
    // 给出300x300大小的pixmap绘画设备
    QPixmap pixmap(300, 300);
    //为pixmap设置背景
    pixmap.fill(Qt::white); // 不设置背景颜色可能会导致最终保存的图片最上面出现一条彩色的像素
    QPainter painter(&pixmap); // 设置绘图设备是pixmap
    painter.setPen(QPen(Qt::cyan));

    painter.drawEllipse(QPoint(150, 150), 100, 100);

    pixmap.save("C:\\Users\\47551\\Desktop\\_cpp\\qt\\011_QPaintDevices\\save\\pixmap.png");

    //! QImage绘图设备 可以对像素进行访问，
    // 不光要给定QImage的大小，还要给出[其图像格式]
    QImage image(300, 300, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter2(&image);
    painter2.setPen(Qt::blue);
    painter2.drawRect(QRect(QPoint(50, 50), QPoint(250, 250)));
    image.save("C:\\Users\\47551\\Desktop\\_cpp\\qt\\011_QPaintDevices\\save\\image.png");

    //! QPicture绘图设备  可以记录和重现绘图指令
    QPicture pic;
    QPainter painter3;
    // 手动设置绘图设备
    painter3.begin(&pic); // 开始在pic绘图设备上绘制

    painter3.setPen(QPen(Qt::red));
    painter3.setBrush(Qt::Dense2Pattern);

    painter3.drawEllipse(QPoint(100, 100), 50, 50);

    painter3.end(); // 结束绘制
    pic.save("C:\\Users\\47551\\Desktop\\_cpp\\qt\\011_QPaintDevices\\save\\picture.qpic");



}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    //! 使用QImage对像素进行修改
    QImage img;
    img.load(":/Images/new.png");
    // qDebug() << img.isNull(); // 判断是否加载图片正确，加载失败会返回true

    // 对像素进行修改
    for (int i = 50; i < 100; ++i) {
        for (int j = 50; j < 100; ++j) {
            QRgb vale = qRgb(255, 0, 0);
            img.setPixel(i, j, vale); // 像素点x, 像素点y, 要设置的像素值
        }
    }

    painter.drawImage(20, 20, img);

    //! 重现QPicture
    QPicture pic;
    // 加载保存的绘图流程文件
    pic.load("C:\\Users\\47551\\Desktop\\_cpp\\qt\\011_QPaintDevices\\save\\picture.qpic");

    painter.drawPicture(0, 300, pic);

}
