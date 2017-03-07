#include "deceivemontor.h"
#include "format.h"
#include "QDebug"

deceiveMontor::deceiveMontor() : QThread()
{
    comFormat = new format();
    serialPort = new mySerialPort();
    comDrive = new drive();
    connect(comDrive, SIGNAL(writeData(QByteArray)), serialPort, SLOT(writeData(QByteArray)));
    connect(serialPort, SIGNAL(receiceData(QByteArray)), this, SLOT(receiveProcess(QByteArray)));
    connect(comDrive, SIGNAL(deviceConnect()), this, SLOT(deviceConnect()));
    connect(comDrive, SIGNAL(deviceDisconnect()), this, SLOT(deviceDisconnect()));
    connect(comDrive, SIGNAL(writeError()), this, SLOT(writeError()));
}

/******************************************
 * @函数说明：线程
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::run()
{
    DataPacket pack;
    while(runFlag)
    {
        pack.nCMD = 0x01;
        pack.nLength = 0;
        emit writeData(comFormat->BuildPack(&pack));

        sleep(2);
    }
}
/******************************************
 * @函数说明：启动串口监测
 * @输入参数：QString name 串口名称
 * @返回参数：打开串口是否成功
 * @修订日期：
******************************************/
bool deceiveMontor::montorSerialPort(QString name)
{
    stopMontor();
    bool result = serialPort->openPortForDef(name);
    if (result == true)
    {
        runFlag = true;
        comDrive->startHearbeat();
    }
    return result;
}
/******************************************
 * @函数说明：关闭监测
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::stopMontor()
{
    terminate();
    runFlag = false;
    serialPort->closePort();
}
/******************************************
 * @函数说明：关闭监测
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::receiveProcess(QByteArray buf)
{
    comDrive->receiveProcess(buf);

}
/******************************************
 * @函数说明：升级设备固件
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
bool deceiveMontor::updateFirmware(QByteArray binByteArray)
{
    return true;
}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::deviceConnect()
{
    emit updateDeviceInfo("Device Connected");
}
/******************************************
 * @函数说明：升级设备固件
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::deviceDisconnect()
{
    emit updateDeviceInfo("Device Disconnected");
}
/******************************************
 * @函数说明：升级设备固件
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::writeError()
{
    emit updateDeviceInfo("Port Error");
}
