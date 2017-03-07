#ifndef DRIVE_H
#define DRIVE_H

#include <QObject>
#include "QTimer"
#include "format.h"
class drive : public QObject
{
    Q_OBJECT
public:
    explicit drive(QObject *parent = 0);
    void startHearbeat();

private:
    QTimer *hearbeatTimer;
    QTimer *retryTimer;
    format *comFormat;
    DataPacket latestPacket;
    uint8_t retryCount;


    void sendPack(DataPacket packet);
    void sendPackAndStartRetry(DataPacket packet);

signals:
    int writeData(QByteArray array);

private slots:
    void retryTimeOut();
    void sendHearbeat();

public slots:
    void receivePacketProcess(DataPacket packet);

};

#endif // DRIVE_H
