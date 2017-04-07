#include "myserialport.h"
#include "QtSerialPort/qserialport.h"
#include "QtSerialPort/qserialportinfo.h"
#include "QDebug"

mySerialPort::mySerialPort(QObject *parent) : QObject(parent)
{
    serialPort = new QSerialPort();

    QObject::connect(serialPort, &QSerialPort::readyRead, this, &mySerialPort::readData);
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/

void mySerialPort::setBaudRate(QString str)
{
    bool ok;

    serialPort->setBaudRate(str.toInt(&ok, 10));
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::setParity(QString str)
{
    if (str == QString::fromLocal8Bit("无校验"))
    {
        serialPort->setParity(QSerialPort::NoParity);
    }
    else if (str == QString::fromLocal8Bit("奇校验"))
    {
        serialPort->setParity(QSerialPort::OddParity);
    }
    else if (str == QString::fromLocal8Bit("偶校验"))
    {
        serialPort->setParity(QSerialPort::EvenParity);
    }
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::setDataBits(QString str)
{
    if (str == "8")
    {
        serialPort->setDataBits(QSerialPort::Data8);
    }
    else if (str == "7")
    {
        serialPort->setDataBits(QSerialPort::Data7);
    }
    else if (str == "6")
    {
        serialPort->setDataBits(QSerialPort::Data6);
    }
    else if (str == "5")
    {
        serialPort->setDataBits(QSerialPort::Data5);
    }
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::setStopBits(QString str)
{
    if (str == "1")
    {
        serialPort->setStopBits(QSerialPort::OneStop);
    }
    else if (str == "1.5")
    {
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if (str == "2")
    {
        serialPort->setStopBits(QSerialPort::TwoStop);
    }
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::setFlowCtrl(QString str)
{
    if (str == "off")
    {
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
    else if (str == "hardware")
    {
        serialPort->setFlowControl(QSerialPort::HardwareControl);
    }
    else if (str == "soft")
    {
        serialPort->setFlowControl(QSerialPort::SoftwareControl);
    }
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
bool mySerialPort::openPort(QString str)
{
    serialPort->setPortName(str);
    qDebug()<<str;
    return serialPort->open(QIODevice::ReadWrite);
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::closePort(void)
{
    serialPort->close();
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void mySerialPort::readData(void)
{
    QByteArray buf;
    buf = serialPort->readAll();
    if (buf.length() != 0)
    {
        qDebug("Sserial port rx %d\r\n", buf.length());
        emit receiceData(buf);
    }
    buf.clear();
}
/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
int mySerialPort::writeData(QByteArray array)
{
    return serialPort->write(array);
}
/******************************************
 * @函数说明：使用默认配置打开串口
 * @输入参数：QString portName 端口名称
 * @返回参数：bool 结果
 * @修订日期：
******************************************/
 bool mySerialPort::openPortForDef(QString portName)
{
     setBaudRate("115200");
     setDataBits("8");
     setFlowCtrl("off");
     setParity("无校验");
     setStopBits("1");
     return openPort(portName);
}
