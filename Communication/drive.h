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
    void receiveProcess(QByteArray buf);

private:
    QTimer *hearbeatTimer;
    QTimer *retryTimer;
    format *comFormat;
    DataPacket latestPacket;
    uint8_t retryCount;
    bool connected;

    void sendPackAndStartRetry(DataPacket packet);
    void receivePacketProcess(DataPacket *packet);

signals:
    int writeData(QByteArray array);    //写入数据
    void deviceConnect();   //设备连接
    void deviceDisconnect();    //设备断开连接
    void writeError();

private slots:
    void retryTimeOut();    //
    void sendHearbeat();

};

#endif // DRIVE_H
