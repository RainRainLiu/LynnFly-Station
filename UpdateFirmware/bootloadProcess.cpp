#include "bootloadProcess.h"
#include "Componemts/crc32.h"
#include "QDebug"

#define COM_CMD_HEARBEAT        0X01
#define COM_CMD_INFO            0X02
#define COM_CMD_ERASURE         0X03
#define COM_CMD_ERASURE_REPORT  0X04
#define COM_CMD_DOWNLOAD_INFO   0X05
#define COM_CMD_DOWNLOAD_DATA   0X06
#define COM_CMD_RUN_FIRMWARE    0X07



bootloadProcess::bootloadProcess(QObject *parent) : QObject(parent)
{
    comFormat = new communicationFormat();
    retryTimer = new QTimer();
    hearbeatTimer = new QTimer();
    connect(retryTimer, SIGNAL(timeout()), this, SLOT(retryTimeOut()));
    connect(hearbeatTimer, SIGNAL(timeout()), this, SLOT(sendHearbeat()));
    connected = false;

}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
bool bootloadProcess::updateFirmware(QByteArray binByteArray, QString version, uint32_t offsetAddr)
{
    firmware = binByteArray;
    firmwarVersion = version;
    firmwareOffsetAddress = offsetAddr;
    erase(firmware.length());
    return true;
}
/******************************************
 * @函数说明：启动心跳
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::startHearbeat()
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
void bootloadProcess::sendHearbeat()
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
void bootloadProcess::sendPackAndStartRetry(DataPacket packet)
{
    latestPacket = packet;
    retryCount = 0;

    if ((emit writeData(comFormat->BuildPack(&packet))) > 0)
    {
        retryTimer->start(RETRY_TIME_OUT_TIME); //启动重发定时器
    }
    else
    {
        emit bootloadEvent(WRITE_ERROR, NULL);
    }
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::retryTimeOut()
{
    retryCount++;
    if (retryCount > 3)
    {
        if (retryTimer->isActive() == true) //终止重试定时器
        {
            retryTimer->stop();
        }
        if (connected == true)
        {
            connected = false;
            emit bootloadEvent(CONNECT_STATE, &connected); //设备断开连接
        }
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
            emit bootloadEvent(WRITE_ERROR, NULL);
        }
    }
}

/******************************************
 * @函数说明：发送数据包，并启动重发
 * @输入参数：DataPacket packet 数据包
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::receivePacketProcess(DataPacket *packet)
{
    //qDebug()<<"CMD";
    //qDebug()<<("%x", packet->nCMD);
    switch(packet->nCMD)
    {
        case COM_CMD_HEARBEAT:
        {
            if (connected == false)
            {
                connected = true;
                emit bootloadEvent(CONNECT_STATE, &connected); //设备连接
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
            qDebug()<<"Earsure ACK";
        }
        break;

        case COM_CMD_ERASURE_REPORT:
        {
            qDebug()<<"Earsure Sucess";
            firmwareInfo(firmware);
        }
        break;

        case COM_CMD_DOWNLOAD_INFO:
        {
            currentPackNum = 0;
            downloadProgress = 0;
            downloadFirmwarePack();

        }
        break;

        case COM_CMD_DOWNLOAD_DATA:
        {
            if (currentPackNum < packetNumber)
            {
                downloadFirmwarePack();
            }

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
void bootloadProcess::receiveProcess(QByteArray buf)
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


void set32Byte(char *buf, uint32_t data)
{
    buf[0] = uint8_t(data & 0xff);
    buf[1] = uint8_t((data >> 8) & 0xff);
    buf[2] = uint8_t((data >> 16) & 0xff);
    buf[3] = uint8_t((data >> 24) & 0xff);
}

/******************************************
 * @函数说明：擦除
 * @输入参数：uint32_t size 大小
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::erase(uint32_t size)
{
    DataPacket pack;
    pack.nCMD = COM_CMD_ERASURE;
    pack.nLength = 4;
    set32Byte((char *)pack.aData, size);
    sendPackAndStartRetry(pack);
}

/******************************************
 * @函数说明：擦除
 * @输入参数：uint32_t size 大小
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::firmwareInfo(QByteArray firmware)
{
    DataPacket pack;
    pack.nCMD = COM_CMD_DOWNLOAD_INFO;
    pack.nLength = 12;
    set32Byte((char *)pack.aData, firmware.length());
    uint32_t crc = CRC32((uint8_t *)firmware.data(), firmware.length());
    set32Byte((char *)&pack.aData[4], crc);

    packetNumber = firmware.length() / DOWNLOAD_FILE_PACK_SIZE;
    if (firmware.length() % DOWNLOAD_FILE_PACK_SIZE)
    {
        packetNumber += 1;
    }
    set32Byte((char *)&pack.aData[8], packetNumber);
    //qDebug()<<("packetNumber = %d", packetNumber);
    sendPackAndStartRetry(pack);
}

/******************************************
 * @函数说明：擦除
 * @输入参数：uint32_t size 大小
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::downloadFirmwarePack()
{
    DataPacket pack;
    pack.nCMD = COM_CMD_DOWNLOAD_DATA;
    pack.nLength = DOWNLOAD_FILE_PACK_SIZE + 4;
    set32Byte((char *)pack.aData, currentPackNum);

    uint8_t *dataBuf = (uint8_t *)firmware.data();
    uint32_t length = 0;

    if ((uint32_t)firmware.length() >= ((currentPackNum + 1) * DOWNLOAD_FILE_PACK_SIZE))
    {
        length = DOWNLOAD_FILE_PACK_SIZE;
    }
    else
    {
        length = firmware.length() - currentPackNum * DOWNLOAD_FILE_PACK_SIZE;
    }

    memcpy(&pack.aData[4], &dataBuf[currentPackNum * DOWNLOAD_FILE_PACK_SIZE], length);

    downloadProgress += length;
    currentPackNum++;
    //qDebug()<<("currentPackNum = %d", currentPackNum);
    emit updateFirmwareProgress(downloadProgress);

    sendPackAndStartRetry(pack);
}

/******************************************
 * @函数说明：擦除
 * @输入参数：uint32_t size 大小
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::runFirmware()
{
    DataPacket pack;
    pack.nCMD = COM_CMD_RUN_FIRMWARE;
    pack.nLength = 0;
    sendPackAndStartRetry(pack);
}

