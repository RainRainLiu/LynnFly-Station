#ifndef BOOTLOAD_H
#define BOOTLOAD_H

#include <QObject>
#include "QTimer"
#include "Componemts/communicationFormat.h"

#define HERARBEAT_TIME_OUT_TIME_SHORT 10
#define HERARBEAT_TIME_OUT_TIME     1000
#define RETRY_TIME_OUT_TIME         200
#define DOWNLOAD_FILE_PACK_SIZE     1024

class bootloadProcess : public QObject
{
    Q_OBJECT
public:
    explicit bootloadProcess(QObject *parent = 0);
    bool updateFirmware(QByteArray binByteArray, QString version, uint32_t offsetAddr);   //更新固件
    void startHearbeat();
    void runFirmware();
    void getInfo();
    void erase(uint32_t size, uint32_t offsetAddress);
    void stopAllTime();
    typedef enum
    {
        WRITE_ERROR,
        CONNECT_STATE,              //设备连接的状态     bool
        EARSE_RESULT,               //擦除结果          bool
        WRITE_INFO_RESULT,          //写入固件信息结果    bool
        DLOWNLOAD_PROGRESS,         //下载固件进度       u32
        DLOWNLOAD_RESULT,           //下载结果
        RUN_FIRMWARE_RESULT,        //运行固件
    }BOOTLOAD_EVENT_T;

private:
    QTimer *hearbeatTimer;  //心跳定时器
    QTimer *retryTimer;     //重发
    communicationFormat *comFormat;      //格式
    DataPacket latestPacket;    //最新发送数据包
    uint8_t retryCount;     //重试次数
    bool connected;         //连接标志
    QByteArray firmware;    //固件
    QString firmwareVersion; //固件版本
    uint32_t firmwareOffsetAddress; //固件偏移地址
    bool downloadFlag;

    uint32_t currentPackNum;
    uint32_t packetNumber;
    uint32_t downloadProgress;

    void sendPackAndStartRetry(DataPacket packet);
    void receivePacketProcess(DataPacket *packet);
    void firmwareInfo(QByteArray firmware, QString version, uint32_t offsetAddress);
    void downloadFirmwarePack();

    void stopRetry();


signals:
    int writeData(QByteArray array);    //数据出口
    void bootloadEvent(bootloadProcess::BOOTLOAD_EVENT_T event, void *arg);
    void bootloadInfo(QString bootloadVersion, QString firmwareVersion, bool firmwareState);

public slots:
    void receiveProcess(QByteArray buf);    //接收数据

private slots:
    void retryTimeOut();
    void sendHearbeat();


};

#endif // BOOTLOAD_H
