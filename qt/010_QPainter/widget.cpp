#include "widget.h"
#include "ui_widget.h"
#include <QPainter> // 画家类
#include <QPen> // 画笔类

#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 点击按钮，手动触发绘图事件,当点击按钮的时候，图片向下移动
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        // 使用update函数做更新
        posY+=20;
        if(posY > this->height()){
            posY=0; // 如果超过屏幕宽度，就重置位置
        }
        update();
    });

    // 使用定时器自动完成上面的操作，
    QTimer * timer = new QTimer(this);
    timer->start(500);
    connect(timer, &QTimer::timeout, [=](){
        posY+=20;
        if(posY>this->height()){
            posY=-200;
        }
        update();
    });
}

Widget::~Widget()
{
    delete ui;
}

// 重写绘画事件
void Widget::paintEvent(QPaintEvent *event){
    // 实例化画家类 参数为指定画布
    QPainter painter(this);
    // 画线
    painter.drawLine(QPoint(0, 0), QPoint(100, 100));
    // 画圆 Ellipse 椭圆 圆心和rx ry
    painter.drawEllipse(QPoint(100, 100), 50, 50);
    // 画矩形
    // 左上点和右下点确定一个矩形区域
    painter.drawRect(QRect(QPoint(100, 100), QPoint(150, 150)));
    // 左上点和QSize(w, h)
    painter.drawRect(QRect(QPoint(), QSize()));
    // 左，上，宽，高
    painter.drawRect(QRect());

    // 画文字，在矩形框中去写字
    painter.drawText(QRect(200, 200, 100, 100), "好好学习");


    // 设置画笔的颜色
    QPen pen(QColor(255 ,0, 0));
    painter.setPen(pen);// 给画家设置笔

    painter.drawLine(QPoint(300, 0), QPoint(300, 300));

    // 设置画笔宽度
    pen.setWidth(3);
    painter.setPen(pen);// 给画家设置笔
    painter.drawLine(QPoint(320, 0), QPoint(320, 300));

    // 设置画笔类型
    /*
     *  Qt::NoPen               0   no line at all. For example, QPainter::drawRect() fills but does not draw any boundary line.
        Qt::SolidLine           1   A plain line.  默认是这个
        Qt::DashLine            2   Dashes separated by a few pixels.
        Qt::DotLine             3   Dots separated by a few pixels.
        Qt::DashDotLine         4   Alternate dots and dashes.
        Qt::DashDotDotLine      5   One dash, two dots, one dash, two dots.
        Qt::CustomDashLine      6   A custom pattern defined using QPainterPathStroker::setDashPattern()
    */
    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);// 给画家设置笔
    painter.drawLine(QPoint(340, 0), QPoint(340, 300));


    // 设置笔刷 用以给图形填充颜色
    /*
     *  Qt::NoBrush             0   No brush pattern.
        Qt::SolidPattern        1   Uniform color.
        Qt::Dense1Pattern       2   Extremely dense brush pattern.
        Qt::Dense2Pattern       3   Very dense brush pattern.
        Qt::Dense3Pattern       4   Somewhat dense brush pattern.
        Qt::Dense4Pattern       5   Half dense brush pattern.
        Qt::Dense5Pattern       6   Somewhat sparse brush pattern.
        Qt::Dense6Pattern       7   Very sparse brush pattern.
        Qt::Dense7Pattern       8   Extremely sparse brush pattern.
        Qt::HorPattern          9   Horizontal lines.
        Qt::VerPattern          10  Vertical lines.
        Qt::CrossPattern        11  Crossing horizontal and vertical lines.
        Qt::BDiagPattern        12  Backward diagonal lines.
        Qt::FDiagPattern        13  Forward diagonal lines.
        Qt::DiagCrossPattern    14  Crossing diagonal lines.
        Qt::LinearGradientPattern   15  Linear gradient (set using a dedicated QBrush constructor).
        Qt::ConicalGradientPattern  17  Conical gradient (set using a dedicated QBrush constructor).
        Qt::RadialGradientPattern   16  Radial gradient (set using a dedicated QBrush constructor).
        Qt::TexturePattern          24  Custom pattern (see QBrush::setTexture())
     *
    */
    QBrush brush; // 可以设置颜色
    brush.setColor(Qt::cyan);
    brush.setStyle(Qt::Dense3Pattern);
    painter.setBrush(brush);

    painter.drawRect(QRect(QPoint(360, 0), QSize(20, 300)));


    //! 高级设置
    //! 1. 抗锯齿
    /*
        QPainter::Antialiasing  0x01Indicates that the engine should antialias edges of primitives if possible.
        QPainter::TextAntialiasing  0x02Indicates that the engine should antialias text if possible. To forcibly disable antialiasing for text, do not use this hint. Instead, set QFont::NoAntialias on your font's style strategy.
        QPainter::SmoothPixmapTransform 0x04Indicates that the engine should use a smooth pixmap transformation algorithm (such as bilinear) rather than nearest neighbor.
        QPainter::VerticalSubpixelPositioning   0x08Allow text to be positioned at fractions of pixels vertically as well as horizontally, if this is supported by the font engine. This is currently supported by Freetype on all platforms when the hinting preference is QFont::PreferNoHinting, and also on macOS. For most use cases this will not improve visual quality, but may increase memory consumption and some reduction in text rendering performance. Therefore, enabling this is not recommended unless the use case requires it. One such use case could be aligning glyphs with other visual primitives. This value was added in Qt 6.1.
        QPainter::LosslessImageRendering    0x40Use a lossless image rendering, whenever possible. Currently, this hint is only used when QPainter is employed to output a PDF file through QPrinter or QPdfWriter, where drawImage()/drawPixmap() calls will encode images using a lossless compression algorithm instead of lossy JPEG compression. This value was added in Qt 5.13.
        QPainter::NonCosmeticBrushPatterns  0x80When painting with a brush with one of the predefined pattern styles, transform the pattern too, along with the object being painted. The default is to treat the pattern as cosmetic, so that the pattern pixels will map directly to device pixels, independently of any active transformations. This value was added in Qt 6.4
     */
    QPainter painter2(this);
    painter2.drawEllipse(QPoint(50, 300), 50, 50);
    painter2.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿， 会导致程序运行速度变慢
    painter2.drawEllipse(QPoint(50, 400), 50, 50);

    //!改变坐标原点
    //! 这里在相同位置绘制了一模一样的三个矩形，如果不改变坐标原点，那么三个矩形会重合在一起
    painter2.drawRect(QRect(200, 300, 50, 50));

    painter2.translate(0 , 70);// 将坐标原点移动位置
    painter2.save(); // 保存画家状态 有一个栈用来保存状态

    painter2.drawRect(QRect(200, 300, 50, 50));

    painter2.translate(0 , 70);// 将坐标原点移动位置
    painter2.restore(); // 还原画家状态，还原至栈顶保存的状态

    painter2.drawRect(QRect(200, 300, 50, 50));

    //! 手动调用绘图事件，点击按钮进行重绘
    QPainter painter3(this);
    // drawPixmap绘制图片，(绘制起始的x位置，y位置，要绘制的图)
    painter3.drawPixmap(400, posY, QPixmap(":/Images/new.png"));


}
















