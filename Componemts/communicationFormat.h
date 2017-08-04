#ifndef COMMUNICATIONFORMAT_H
#define COMMUNICATIONFORMAT_H
#include <QObject>

#define PACKET_DATA_LENGTH  1200

#define COM_CMD1_BOOT                   0X00
#define COM_CMD1_DEBUG_PRINTF           0X01
#define COM_CMD1_REMOTE_CONTROL         0X02
#define COM_CMD1_GESTURE_OUTPUT         0X03

#define COM_CMD2_BOOT_HEARBEAT          0X01
#define COM_CMD2_BOOT_INFO              0X02
#define COM_CMD2_BOOT_ERASURE           0X03
#define COM_CMD2_BOOT_ERASURE_REPORT    0X04
#define COM_CMD2_BOOT_DOWNLOAD_INFO     0X05
#define COM_CMD2_BOOT_DOWNLOAD_DATA     0X06
#define COM_CMD2_BOOT_RUN_FIRMWARE      0X07



#define COM_CMD2_GESTURE_OUTPUT_GYRO    0X01
#define COM_CMD2_GESTURE_OUTPUT_ACC     0X02
#define COM_CMD2_GESTURE_OUTPUT_ANGLE   0X03




typedef struct
{
    uint8_t nCMD1;
    uint8_t nCMD2;
    uint16_t nLength;
    uint8_t aData[PACKET_DATA_LENGTH];
}DataPacket;

class communicationFormat : public QObject
{
    Q_OBJECT
public:
    explicit communicationFormat(QObject *parent = 0);
    DataPacket* Parsing(uint8_t inData);   //解析
    QByteArray BuildPack(DataPacket *pPack);

    uint8_t ErrorNumber;
private:
    void ReceiveError();

    uint8_t  step;
    uint8_t lastData;
    uint32_t count;
    DataPacket packet;
    virtual void PackProcess(DataPacket *packet);

signals:
    void OutData(QByteArray byteData);

public slots:
    void InData(QByteArray byteData);

};

#endif // COMMUNICATIONFORMAT_H
