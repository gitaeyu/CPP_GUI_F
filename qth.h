#ifndef QTH_H
#define QTH_H

#include <QThread>

class Timestamp : public QThread
{
    Q_OBJECT
public:
    explicit Timestamp(QObject* parent = 0);
private:
    void run();				// QThread 실행
signals:
    void setTime();
};

class NewAlarm : public QThread
{
    Q_OBJECT
public:
    explicit NewAlarm(QObject* parent = 0);
private:
    void run();				// QThread 실행
signals:
    void checkAlarm();
};

class LabelFlash : public QThread
{
    Q_OBJECT
public:
    explicit LabelFlash(QObject* parent = 0);
private:
    void run();				// QThread 실행
signals:
    void labelsignal();
};

#endif // QTH_H
