#include "deceivemontor.h"
#include "format.h"
#include "QDebug"

deceiveMontor::deceiveMontor() : QThread()
{
    comFormat = new format();
    serialPort = new mySerialPort();
    connect(this, SIGNAL(writeData(QByteArray)), serialPort, SLOT(writeData(QByteArray)));
    connect(serialPort, SIGNAL(receiceData(QByteArray)), this, SLOT(receiveProcess(QByteArray)));
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
        start();
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
    DataPacket *packet = NULL;
    for (int i = 0; i < buf.length(); i++)
    {
        packet =  comFormat->Parsing((uint8_t)buf[i]);
        if (packet != NULL)
        {
            qDebug()<<packet->nCMD;
        }
    }
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
