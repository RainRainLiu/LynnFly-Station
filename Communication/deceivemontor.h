#ifndef DECEIVEMONTOR_H
#define DECEIVEMONTOR_H

#include <QThread>
#include "Componemts/myserialport.h"
#include "format.h"

class deceiveMontor : public QThread
{
    Q_OBJECT
public:
    explicit deceiveMontor();
    void run();
    bool montorSerialPort(QString name);
    void stopMontor();
    bool updateFirmware(QByteArray binByteArray);


private:
    format *comFormat;
    mySerialPort *serialPort;
    bool runFlag;


signals:
    int writeData(QByteArray array);
    void updateFirmwareProgress(QString str, uint32_t progress, bool end);

public slots:
    void receiveProcess(QByteArray buf);
};

#endif // DECEIVEMONTOR_H
