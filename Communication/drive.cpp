#include "drive.h"
#include "format.h"
#include "QDebug"

#define HERARBEAT_TIME_OUT_TIME     500
#define RETRY_TIME_OUT_TIME         500

#define COM_CMD_HEARBEAT        0X01
#define COM_CMD_INFO            0X02
#define COM_CMD_ERASURE         0X03
#define COM_CMD_ERASURE_REPORT  0X04
#define COM_CMD_DOWNLOAD_INFO   0X05
#define COM_CDM_DOWNLOAD_DATA   0X06


drive::drive(QObject *parent) : QObject(parent)
{
    comFormat = new format();
    retryTimer = new QTimer();
    hearbeatTimer = new QTimer();
    connect(retryTimer, SIGNAL(timeout()), this, SLOT(retryTimeOut()));
    connect(hearbeatTimer, SIGNAL(timeout()), this, SLOT(sendHearbeat()));
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

    packet.nCMD = COM_CMD_HEARBEAT;
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

    if ((emit writeData(comFormat->BuildPack(&packet))) > 0)
    {
        retryTimer->start(RETRY_TIME_OUT_TIME); //启动重发定时器
    }
    else
    {
        emit writeError();
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
    if (retryCount > 3)
    {
        if (retryTimer->isActive() == true) //终止重试定时器
        {
            retryTimer->stop();
        }
        emit deviceDisconnect(); //设备断开连接
        connected = false;
        hearbeatTimer->start();
    }
    else
    {
        if ((emit writeData(comFormat->BuildPack(&latestPacket))) > 0)
        {
            retryTimer->start(RETRY_TIME_OUT_TIME); //启动重发定时器
        }
        else
        {
            emit writeError();
        }
    }
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::receivePacketProcess(DataPacket *packet)
{
    switch(packet->nCMD)
    {
        case COM_CMD_HEARBEAT:
        {
            if (connected == false)
            {
                connected = true;
                emit deviceConnect();
            }
            startHearbeat();
        }
        break;

        case COM_CMD_INFO:
        {

        }
        break;

        case COM_CMD_ERASURE:
        {

        }
        break;

        case COM_CMD_ERASURE_REPORT:
        {

        }
        break;

        case COM_CMD_DOWNLOAD_INFO:
        {

        }
        break;

        case COM_CDM_DOWNLOAD_DATA:
        {

        }
        break;

    }
    retryTimer->stop();
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::receiveProcess(QByteArray buf)
{
    DataPacket *packet = NULL;
    for (int i = 0; i < buf.length(); i++)
    {
        packet =  comFormat->Parsing((uint8_t)buf[i]);
        if (packet != NULL)
        {
            receivePacketProcess(packet);
        }
    }
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void drive::sendFileInfo()




