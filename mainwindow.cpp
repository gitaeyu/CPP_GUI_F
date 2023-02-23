 #include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qth.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // UI 생성
    ui->setupUi(this);
    ui->Main_stk->setCurrentIndex(0);
    ui->Lock_lbl->hide();
    // timestamp Qthread 생성 및 시작
    timestamp = new Timestamp(this);
    connect(timestamp, SIGNAL(setTime()), this, SLOT(showTime()));
    timestamp->start();
    // new alaram Qthread 생성 및 시작
    newalarm = new NewAlarm(this);
    connect(newalarm, SIGNAL(checkAlarm()), this, SLOT(showAlarm()));
    newalarm->start();
    // player 초기화
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    // alarm label flash Qthread 생성 및 시작
    labelflash = new LabelFlash(this);
    connect(labelflash, SIGNAL(labelsignal()), this, SLOT(blink()));
    labelflash->start();
    //보안레벨 1로 초기화
    mSecurityLevel = 1;
    ui->AlarmColor->hide();
    // Lock Status 초기화
    Lock_Status = false;
    Alarm_Status = false;
    // isolationControls 구조체 초기화
    isolationControls[0] = {ui->Zone1_isol_btn, ui->Zone1_test_btn,ui->Zone1_isol_lbl, false};
    isolationControls[1] = {ui->Zone2_isol_btn, ui->Zone2_test_btn,ui->Zone2_isol_lbl, false};
    isolationControls[2] = {ui->Zone3_isol_btn, ui->Zone3_test_btn,ui->Zone3_isol_lbl, false};
    isolationControls[3] = {ui->Zone4_isol_btn, ui->Zone4_test_btn,ui->Zone4_isol_lbl, false};
    // 구조체의 버튼들 connect 함수 연결
    for(int i =0; i<4;i++)
    {
        connect(isolationControls[i].isol_btn, &QPushButton::clicked, [=]() {
            clickIsolation(i);
        });
    }
    for(int i =0; i<4;i++)
    {
        connect(isolationControls[i].test_btn, &QPushButton::clicked, [=]() {
            clickTest(i);
        });
    }
    //DB 연결
    db = QSqlDatabase::addDatabase("QMYSQL");
       db.setHostName("localhost");
       db.setDatabaseName("firealarmsys");
       db.setUserName("root");
       db.setPassword("00000000");

       if (!db.open()) {
           qDebug() << "Database error occurred";
        }
       else {
           qDebug() << "Database open Success";
        }


}

MainWindow::~MainWindow()
{
    delete ui;
    delete timestamp;
    delete newalarm;
    delete labelflash;
    delete player;
    delete audioOutput;

}


void MainWindow::showTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    ui->Time_stamp_lbl->setText(currentDateTimeString);
    ui->Time_stamp_lbl->setAlignment(Qt::AlignCenter);
}

void MainWindow::showAlarm()
{
    ui->Main_TW->clearContents();
    QString queryStr; //쿼리문 전달할 변수
     queryStr=QString("select * from log where AckState = 0");
     QSqlQuery query; //데이터베이스에 전달할 쿼리변수
     query.prepare(queryStr); //준비
     query.exec();  //실행
     ui->Main_TW->setRowCount(0);
     int row = 0;
     while(query.next())
     {
         ui->Main_TW->insertRow(row);
         ui->Main_TW->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
         ui->Main_TW->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
         ui->Main_TW->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
         ui->Main_TW->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

         row++;
     };
     ui->Main_TW->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

}

void MainWindow::blink()
{
    if((ui->Main_TW->rowCount()>0)&&(ui->AlarmColor->isHidden())){
        ui->AlarmColor->show();
    }
    else if((ui->Main_TW->rowCount()>0)&&!(ui->AlarmColor->isHidden()))
    {
        ui->AlarmColor->hide();
    }
    if(ui->Main_TW->rowCount()>0 && !Alarm_Status)
    {
        play_Alarm_sound();
        Alarm_Status = true;
    }
}
void MainWindow::on_Home_btn_clicked()
{
    if(!Lock_Status)
    ui->Main_stk->setCurrentIndex(0);
    else
    Show_Lock_lbl();
}


void MainWindow::on_Menu_btn_clicked()
{
    if(!Lock_Status)
    ui->Main_stk->setCurrentIndex(1);
    else
    Show_Lock_lbl();
}


void MainWindow::on_Setting_btn_clicked()
{
    if(!Lock_Status)
    ui->Main_stk->setCurrentIndex(2);
    else
    Show_Lock_lbl();
}


void MainWindow::on_Isolation_btn_clicked()
{
    if(mSecurityLevel==3)
    ui->Main_stk->setCurrentIndex(3);
}


void MainWindow::on_Test_btn_clicked()
{
    if(mSecurityLevel>1)
    ui->Main_stk->setCurrentIndex(4);
}


void MainWindow::on_Log_btn_clicked()
{
    ui->Main_stk->setCurrentIndex(5);
    ui->log_TW->clearContents();
    QString queryStr; //쿼리문 전달할 변수
     queryStr=QString("select * from log");
     QSqlQuery query; //데이터베이스에 전달할 쿼리변수
     query.prepare(queryStr); //준비
     query.exec();  //실행
     ui->log_TW->setRowCount(0);
     int row = 0;
     while(query.next())
     {
         ui->log_TW->insertRow(row);
         ui->log_TW->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
         ui->log_TW->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
         ui->log_TW->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
         ui->log_TW->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
         row++;
     };
     ui->log_TW->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}



void MainWindow::on_Lock_btn_clicked()
{
    if(!Lock_Status)
    {
        ui->Lock_btn->setText("Unlock");
        ui->Lock_Status_lbl->setText("Lock_status : Lock");
        ui->Main_stk->setCurrentIndex(0);
        Lock_Status = true;
    }
    else
    {
        ui->Lock_btn->setText("Lock");
        ui->Lock_Status_lbl->setText("Lock_status : Unlock");
        Lock_Status = false;
    }
}




void MainWindow::clickIsolation(int zoneIndex)
{
    IsolationControl &isolationControl = isolationControls[zoneIndex];

    if (isolationControl.status == false) {
        isolationControl.status = true;
        isolationControl.label->setText("Isolated");

    } else {
        // turn off isolation
        isolationControl.status = false;
        isolationControl.label->setText("Normal");
    }
}

void MainWindow::clickTest(int zoneIndex)
{
    IsolationControl &isolationControl = isolationControls[zoneIndex];

    if (isolationControl.status == false) {
       // DB insert
       QString queryStr; //쿼리문 전달할 변수
        queryStr=QString("insert into log (Location,State,Timestamp,AckState)values ('%1','Fire','%2',0)")
               .arg(isolationControl.test_btn->text())
               .arg(ui->Time_stamp_lbl->text()); //쿼리문 저장
        QSqlQuery query; //데이터베이스에 전달할 쿼리변수
        query.prepare(queryStr); //준비
        query.exec();  //실행
        Alarm_Status = false;
    }
}

void MainWindow::on_Acknowledge_btn_clicked()
{
    if(!Lock_Status){
        for(int i=0 ; i < ui->Main_TW->rowCount();i++)
        {
            QString index;
            index = QString("%1").arg(ui->Main_TW->item(i,0)->text());
            int number = index.toInt();
            QString queryStr;
            queryStr = QString("Update log set AckState = 1 where Number = %1").arg(number);
            QSqlQuery query;
            query.prepare(queryStr);
            query.exec();
        }
        ui->AlarmColor->hide();
        player->stop();
        Alarm_Status = false;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)

{
    db.close();
    event->accept();
}



void MainWindow::on_Setting_lvl1_btn_clicked()
{
    mSecurityLevel = 1;
    ui->Security_level_lbl->setText("Security Level : Level 1");

}


void MainWindow::on_Setting_lvl2_btn_clicked()
{
    mSecurityLevel = 2;
    ui->Security_level_lbl->setText("Security Level : Level 2");
}


void MainWindow::on_Setting_lvl3_btn_clicked()
{
    mSecurityLevel = 3;
    ui->Security_level_lbl->setText("Security Level : Level 3");
}
void MainWindow::Show_Lock_lbl()
{
    ui->Lock_lbl->setWindowOpacity(0);
    ui->Lock_lbl->show();


    QPropertyAnimation *animation = new QPropertyAnimation(ui->Lock_lbl, "geometry",this);
    animation->setDuration(1000);
    animation->setStartValue(QRect(300, 200, 200, 200));
    animation->setEndValue(QRect(300,-200, 200, 200));
    animation->setEasingCurve(QEasingCurve::OutQuad);

    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        ui->Lock_lbl->hide();
        delete animation;
    });
    qDebug() << "finish ";
}

void MainWindow::play_Alarm_sound()
{
    if(!Alarm_Status){
    qDebug()<<player->mediaStatus();
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile("/myPy/pycharm/Kitty/0220_CPP_GUI/Firealarm.wav"));
    audioOutput->setVolume(50);
    player->play();
    connect(player, &QMediaPlayer::playbackStateChanged,[=] {Alarm_Status = false;});
    }
}

void MainWindow::on_Download_Log_btn_clicked()
{
    QString queryStr;
    queryStr = QString("select * from log");
    QSqlQuery query;
    query.prepare(queryStr);
    query.exec();
    QFile file("/myPy/pycharm/Kitty/0220_CPP_GUI/data.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        while (query.next()) {
            QString Number = query.value(0).toString();
            QString Location = query.value(1).toString();
            QString State = query.value(2).toString();
            QString Timestamp = query.value(3).toString();
            out << Number << "," << Location << "," << State << "," << Timestamp <<"\n";
        }
        file.close();
    }
}
