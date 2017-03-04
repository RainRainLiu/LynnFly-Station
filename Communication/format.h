#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>

#define PACKET_DATA_LENGTH  1024


typedef struct
{
    uint8_t nCMD;
    uint16_t nLength;
    uint8_t aData[PACKET_DATA_LENGTH];
}DataPacket;

class format : public QObject
{
    Q_OBJECT
public:

    explicit format(QObject *parent = 0);
    DataPacket* Parsing(uint8_t inData);   //解析
    QByteArray BuildAndSendPack(DataPacket *pPack);

    uint8_t ErrorNumber;

private:
    void ReceiveError();

    uint8_t  step;
    uint8_t lastData;
    uint32_t count;
    uint8_t checkSum;
    DataPacket packet;

signals:
    //void packetComplete(struct DataPacket packet);

public slots:
};

#endif // FORMAT_H
