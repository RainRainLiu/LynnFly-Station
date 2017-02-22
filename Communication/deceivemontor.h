#ifndef DECEIVEMONTOR_H
#define DECEIVEMONTOR_H

#include <QObject>
#include "Componemts/myserialport.h"

class deceiveMontor : public QObject
{
    Q_OBJECT
public:
    explicit deceiveMontor(QObject *parent = 0, mySerialPort serial);

private:
    mySerialPort *serialPort;

signals:

public slots:
};

#endif // DECEIVEMONTOR_H
