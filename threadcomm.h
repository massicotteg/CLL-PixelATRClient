#ifndef THREADCOMM_H
#define THREADCOMM_H

#include <QThread>

class ThreadComm : public QThread
{
    Q_OBJECT
public:
    explicit ThreadComm(QObject *parent = 0);

protected:
    void run();
signals:

public slots:
};

#endif // THREADCOMM_H
