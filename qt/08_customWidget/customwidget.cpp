#include "customwidget.h"
#include "ui_customwidget.h"

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomWidget)
{
    ui->setupUi(this);

    // 自定义控件，
    // 1. 创建自定义控件，添加新文件 new->设计师界面类,将需要封装的页面设置好

    // 2. 设置封装控件的逻辑功能 提升数字就修改进度条
    // connect(ui->spinBox, &QSpinBox::valueChanged, [=](int value){
    //     ui->horizontalSlider->setValue(value);
    // });

    connect(ui->spinBox, &QSpinBox::valueChanged, ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);

    // 3. 在主页面(widget)中，使用自定义控件
        // 拖拽一个widget作为容器，右键提升，输入类名，点击添加，点击提升

}

// 3. 为自定义控件向外提供一些方法接口  调用见widget.cpp中两个按钮的实现功能
void CustomWidget::setNum(int num){
    ui->spinBox->setValue(num);
}

int CustomWidget::getNum(){
    return ui->spinBox->value();
}


CustomWidget::~CustomWidget()
{
    delete ui;
}
