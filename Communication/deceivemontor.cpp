#include "deceivemontor.h"
#include "format.h"

deceiveMontor::deceiveMontor() : QThread()
{
    serialPort = new mySerialPort();
    connect(this, SIGNAL(writeData(QByteArray)), serialPort, SLOT(writeData(QByteArray)));
}

/******************************************
 * @函数说明：线程
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void deceiveMontor::run()
{
    format *f = new format();
    DataPacket pack;
    while(runFlag)
    {
        pack.nCMD = 0x01;
        pack.nLength = 0;
        emit writeData(f->BuildAndSendPack(&pack));

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
