#ifndef COMMUNICATIONFORMAT_H
#define COMMUNICATIONFORMAT_H
#include <QObject>

#define PACKET_DATA_LENGTH  1200

typedef struct
{
    uint8_t nCMD;
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

signals:

public slots:
};

#endif // COMMUNICATIONFORMAT_H
