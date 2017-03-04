#ifndef DECEIVEMONTOR_H
#define DECEIVEMONTOR_H

#include <QThread>
#include "Componemts/myserialport.h"

class deceiveMontor : public QThread
{
    Q_OBJECT
public:
    explicit deceiveMontor();
    void run();
    bool montorSerialPort(QString name);
    void stopMontor();

private:
    mySerialPort *serialPort;
    bool runFlag;

signals:
    int writeData(QByteArray array);

public slots:
};

#endif // DECEIVEMONTOR_H
