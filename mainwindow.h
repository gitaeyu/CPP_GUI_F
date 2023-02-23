#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QtMultimedia>
#include <QMediaPlayer>
#include "qth.h"
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void showTime();

    void showAlarm();

    void on_Home_btn_clicked();

    void on_Menu_btn_clicked();

    void on_Setting_btn_clicked();

    void on_Isolation_btn_clicked();

    void on_Test_btn_clicked();

    void on_Log_btn_clicked();

    void on_Lock_btn_clicked();

    void clickIsolation(int zoneIndex);

    void clickTest(int zoneIndex);

    void on_Acknowledge_btn_clicked();

    void closeEvent(QCloseEvent *event);

    void blink();

    void on_Setting_lvl1_btn_clicked();

    void on_Setting_lvl2_btn_clicked();

    void on_Setting_lvl3_btn_clicked();

    void Show_Lock_lbl();

    void play_Alarm_sound();



    void on_Download_Log_btn_clicked();

private:
    Ui::MainWindow *ui;
    Timestamp* timestamp;
    NewAlarm* newalarm;
    LabelFlash* labelflash;
    // 화면 Lock을 위한 bool 변수 선언
    bool Lock_Status;
    bool Alarm_Status;
    // 구조체 선언
    struct IsolationControl {
        QPushButton* isol_btn; //IsolationControl.isol_btn 이런식으로 사용함
        QPushButton* test_btn;
        QLabel* label;
        bool status;
    };
    //구조체 4개를 담기 위한 배열 선언
    IsolationControl isolationControls[4];

    // 보안레벨
    int mSecurityLevel;
    QPropertyAnimation *animation;
    QMediaPlayer * player;
    QAudioOutput * audioOutput;
    QSqlDatabase db;

};
#endif // MAINWINDOW_H
