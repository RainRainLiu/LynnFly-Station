#include "drive.h"
#include "format.h"

#define HERARBEAT_TIME_OUT_TIME     1000
#define RETRY_TIME_OUT_TIME         500

#define CMD_HEARBEAT    0X01


drive::drive(QObject *parent) : QObject(parent)
{
    comFormat = new format();
    retryTimer = new QTimer();
    hearbeatTimer = new QTimer();
    connect(retryTimer, SIGNAL(timeout()), this, SLOT(retryTimeOut));
    connect(hearbeatTimer, SIGNAL(timeout()), this, SLOT(sendHearbeat));
}

/******************************************
 * @函数说明：启动心跳
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::startHearbeat()
{
    if (hearbeatTimer->isActive() == false)
    {
        hearbeatTimer->start(HERARBEAT_TIME_OUT_TIME);
    }
}

/******************************************
 * @函数说明：启动心跳
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::sendHearbeat()
{
    hearbeatTimer->stop();

    DataPacket packet;

    packet.nCMD = CMD_HEARBEAT;
    packet.nLength = 0;

    sendPackAndStartRetry(packet);
}
/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::sendPackAndStartRetry(DataPacket packet)
{
    latestPacket = packet;
    retryCount = 0;
    sendPack(packet);
}
/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::sendPack(DataPacket packet)
{
    latestPacket = packet;
    int length =  emit writeData(comFormat->BuildPack(&packet));

    if (length > 0)
    {
        if (retryTimer->isActive() == true)
        {
            retryTimer->stop();
        }
        retryTimer->start(RETRY_TIME_OUT_TIME); //启动重发定时器
    }
}
/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::retryTimeOut()
{
    retryCount++;
    sendPack(latestPacket);
    if (retryCount > 3)
    {

    }
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::receivePacketProcess(DataPacket packet)
{
    switch(packet.nCMD)
    {
        case CMD_HEARBEAT:
        {
            startHearbeat();
        }
        break;
    }
}



