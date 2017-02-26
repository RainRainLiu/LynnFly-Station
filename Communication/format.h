#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>

class format : public QObject
{
    Q_OBJECT
public:
    explicit format(QObject *parent = 0);
    void PacketParsing(uint8_t data);
    uint8_t CMD;
    uint8_t Data[1024];
    uint8_t ErrorNumber;

private:
    void ReceiveError();
    uint8_t  step;
    uint32_t count;
    uint32_t length;
    uint16_t check_sum;

signals:

public slots:
};

#endif // FORMAT_H
