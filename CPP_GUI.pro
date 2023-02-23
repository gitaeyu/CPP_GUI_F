QT       += core gui sql multimedia multimediawidgets



# MySQL
#INCLUDEPATH += "C:/Program Files/MySQL/MySQL Server 8.0/include"
#LIBS += -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" -lmysql
#LIBS += -L"C:/Program Files/MySQL/Connector C++ 8.0/lib" -lmysqlcppconn
#INCLUDEPATH += "C:/Program File/MySQL/Connector C++ 8.0/include"
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qth.cpp

HEADERS += \
    mainwindow.h \
    qth.h

FORMS += \
    mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
