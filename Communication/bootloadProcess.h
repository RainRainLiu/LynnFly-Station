#ifndef BOOTLOAD_H
#define BOOTLOAD_H

#include <QObject>
#include "QTimer"
#include "format.h"


#define HERARBEAT_TIME_OUT_TIME     500
#define RETRY_TIME_OUT_TIME         500
#define DOWNLOAD_FILE_PACK_SIZE     1024

class bootloadProcess : public QObject
{
    Q_OBJECT
public:
    explicit bootloadProcess(QObject *parent = 0);
    bool updateFirmware(QByteArray binByteArray);   //更新固件
    void startHearbeat();
    void runFirmware();

private:
    QTimer *hearbeatTimer;  //心跳定时器
    QTimer *retryTimer;     //重发
    format *comFormat;      //格式
    DataPacket latestPacket;    //最新发送数据包
    uint8_t retryCount;     //重试次数
    bool connected;         //连接标志
    QByteArray firmware;    //固件
    uint32_t currentPackNum;
    uint32_t packetNumber;
    uint32_t downloadProgress;

    void sendPackAndStartRetry(DataPacket packet);
    void receivePacketProcess(DataPacket *packet);
    void erase(uint32_t size);
    void firmwareInfo(QByteArray firmware);
    void downloadFirmwarePack();

signals:
    int writeData(QByteArray array);    //数据出口
    void updateFirmwareProgress(uint32_t progress); //更新固件进度
    void deviceConnect();       //设备连接
    void deviceDisconnect();    //设备断开
    void writeError();          //写错误

public slots:
    void receiveProcess(QByteArray buf);    //接收数据

private slots:

    void sendHearbeat();
    void retryTimeOut();

};

#endif // BOOTLOAD_H
