#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include "QSerialPort"


class mySerialPort : public QObject
{
    Q_OBJECT

private:

    QSerialPort *serialPort;
    void readData(void);

public:
    explicit mySerialPort(QObject *parent = 0);
    void setBaudRate(QString str);
    void setParity(QString str);
    void setDataBits(QString str);
    void setStopBits(QString str);
    void setFlowCtrl(QString str);
    bool openPort(QString str);
    void closePort(void);
    bool openPortForDef(QString portName);


signals:
    void receiceData(QByteArray buf);

public slots:
};

#endif // MYSERIALPORT_H
