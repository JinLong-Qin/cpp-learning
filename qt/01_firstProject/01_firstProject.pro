QT       += core gui  #! QT包含的模块，目前有core和gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  #! 如果QT版本大于4，就讲widgets模块包含进来，因为widgets在qt5以后被从gui中分离出来了。这一行是用来兼容qt5以下的代码的

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 新版本的.pro文件中没有这个，开始
# TARGET=01_firstProject # 生成的.exe程序的名称
# TEMPLATE=app # 模板，应用程序的模板 application
# 新版本的.pro文件中没有这个，结束


# 源文件，在创建文件之后，会追加到这里，不用自己添加
SOURCES += \
    main.cpp \
    mypushbutton.cpp \
    mywidget.cpp

# 头文件，会自动添加新的头文件
HEADERS += \
    mypushbutton.h \
    mywidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
