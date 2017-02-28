#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>

#define PACKET_DATA_LENGTH  1024


class format : public QObject
{
    Q_OBJECT
public:
    struct DataPacket
    {
        uint8_t nCMD;
        uint8_t nLength;
        uint8_t aData[PACKET_DATA_LENGTH];
    };
    typedef void *(SendFun)(uint8_t *buff, uint32_t nLength);

    explicit format(QObject *parent = 0, SendFun *fun = NULL);
    void Parsing(uint8_t inData);   //解析
    void BuildAndSendPack(struct DataPacket *pPack);

    uint8_t ErrorNumber;

private:
    void ReceiveError();

    uint8_t  step;
    uint8_t lastData;
    uint32_t count;
    uint8_t checkSum;
    DataPacket packet;
    SendFun *sendFun;

signals:
    void packetComplete(struct DataPacket packet);

public slots:
};

#endif // FORMAT_H