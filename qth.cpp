#include "qth.h"

Timestamp::Timestamp(QObject *parent) : QThread(parent)
{

}
void Timestamp::run()
{
    while(1)
    {
        emit setTime();
        sleep(1);
    }
}

NewAlarm::NewAlarm(QObject *parent) : QThread(parent)
{

}
void NewAlarm::run()
{
    while(1)
    {
        emit checkAlarm();
        sleep(1);
    }
}

LabelFlash::LabelFlash(QObject *parent) : QThread(parent)
{

}
void LabelFlash::run()
{
    while(1)
    {
        emit labelsignal();
        sleep(1);
    }
}
