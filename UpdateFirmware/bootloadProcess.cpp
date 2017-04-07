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
    downloadFlag = false;

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
    firmwareVersion = version;
    firmwareOffsetAddress = offsetAddr;
    downloadFlag = true;

    erase(firmware.length(), offsetAddr);
    return true;
}

void bootloadProcess::stopAllTime()
{
    retryTimer->stop();
    hearbeatTimer->stop();
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
        if (connected == true)
        {
            hearbeatTimer->start(HERARBEAT_TIME_OUT_TIME);
        }
        else
        {
            hearbeatTimer->start(HERARBEAT_TIME_OUT_TIME_SHORT);
        }

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
    if (connected == true)
    {
        hearbeatTimer->stop();
    }

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
    qDebug()<<"Retry Time Out";
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
        startHearbeat();
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
void bootloadProcess::stopRetry()
{
    if (retryTimer->isActive() == true) //终止重试定时器
    {
        retryTimer->stop();
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
    qDebug("CMD %x ", packet->nCMD);
    stopRetry();
    switch(packet->nCMD)
    {
        case COM_CMD_HEARBEAT:
        {
            if (connected == false)
            {
                connected = true;
                getInfo();
                emit bootloadEvent(CONNECT_STATE, &connected); //设备连接
            }
            startHearbeat();
        }
        break;

        case COM_CMD_INFO:
        {
            QString bootloadVersion = QString::fromLocal8Bit((const char *)&packet->aData[1], (int)packet->aData[0]);
            QString firmwareVersion = QString::fromLocal8Bit((const char *)&packet->aData[packet->aData[0] + 2], (int)packet->aData[packet->aData[0] + 1]);
            if (packet->aData[packet->nLength - 1] == 0)
            {
                emit bootloadInfo( bootloadVersion, firmwareVersion, true);
            }
            else
            {
                emit bootloadInfo(bootloadVersion, firmwareVersion, false);
            }
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
            bool ok;
            if (packet->aData[0] == 0)
            {
                ok = true;
            }
            else
            {
                ok = false;
            }
            emit bootloadEvent(EARSE_RESULT, &ok);
            if (downloadFlag == true)
            {
                firmwareInfo(firmware, firmwareVersion, firmwareOffsetAddress);
            }
        }
        break;

        case COM_CMD_DOWNLOAD_INFO:
        {
            bool ok;
            if (packet->aData[0] == 0)
            {
                ok = true;
                qDebug()<<"DOWNLOAD_INFO Sucess";
                downloadFlag = false;
                downloadProgress = 0;
                packetNumber = firmware.length() / DOWNLOAD_FILE_PACK_SIZE;
                if (firmware.length() % DOWNLOAD_FILE_PACK_SIZE > 0)
                {
                    packetNumber += 1;
                }
                currentPackNum = 0;
                downloadFirmwarePack();
            }
            else
            {
                ok = false;
            }
            emit bootloadEvent(WRITE_INFO_RESULT, &ok);
        }
        break;

        case COM_CMD_DOWNLOAD_DATA:
        {
            bool ok;
            if (packet->aData[0] == 0)
            {
                if (currentPackNum < packetNumber)
                {
                    downloadFirmwarePack();
                    emit bootloadEvent(DLOWNLOAD_PROGRESS, &downloadProgress);
                }
                else        //下载完成
                {
                    ok = true;
                    emit bootloadEvent(DLOWNLOAD_PROGRESS, &downloadProgress);
                    emit bootloadEvent(DLOWNLOAD_RESULT, &ok);
                }

            }
            else
            {
                ok = false;
                emit bootloadEvent(DLOWNLOAD_RESULT, &ok);
            }

        }
        break;

        case COM_CMD_RUN_FIRMWARE:
        {
            bool ok;
            if (packet->aData[0] == 0)
            {
                ok = true;
                emit bootloadEvent(RUN_FIRMWARE_RESULT, &ok);
            }
            else
            {
                ok = false;
                emit bootloadEvent(RUN_FIRMWARE_RESULT, &ok);
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
void bootloadProcess::erase(uint32_t size, uint32_t offsetAddress)
{
    DataPacket pack;
    pack.nCMD = COM_CMD_ERASURE;
    pack.nLength = 8;
    set32Byte((char *)pack.aData, size);
    set32Byte((char *)&pack.aData[4], offsetAddress);
    sendPackAndStartRetry(pack);
}

/******************************************
 * @函数说明：擦除
 * @输入参数：uint32_t size 大小
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::firmwareInfo(QByteArray firmware, QString version, uint32_t offsetAddress)
{
    DataPacket pack;
    pack.nCMD = COM_CMD_DOWNLOAD_INFO;
    pack.nLength = 0;
    set32Byte((char *)pack.aData, firmware.length());       //固件长度
    pack.nLength += 4;
    uint32_t crc = CRC32((uint8_t *)firmware.data(), firmware.length());    //CRC校验码
    set32Byte((char *)&pack.aData[4], crc);
    pack.nLength += 4;

    set32Byte((char *)&pack.aData[8], offsetAddress);   //偏移地址
    pack.nLength += 4;

    pack.aData[pack.nLength++] = (uint8_t)(version.length() & 0xff);    //版本号长度

    memcpy(&pack.aData[pack.nLength], version.toLocal8Bit().data(), version.length() & 0xff);//版本号
    pack.nLength += (uint8_t)(version.length() & 0xff);

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
    pack.nLength = DOWNLOAD_FILE_PACK_SIZE + 8;


    set32Byte((char *)pack.aData, packetNumber);


    set32Byte((char *)&pack.aData[4], currentPackNum);

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

    memcpy(&pack.aData[8], &dataBuf[currentPackNum * DOWNLOAD_FILE_PACK_SIZE], length);

    downloadProgress += length;
    currentPackNum++;
    qDebug()<<("currentPackNum = %d", currentPackNum);

    sendPackAndStartRetry(pack);

}

/******************************************
 * @函数说明：运行固件
 * @输入参数：无
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
/******************************************
 * @函数说明：运行固件
 * @输入参数：无
 * @返回参数：无
 * @修订日期：
******************************************/
void bootloadProcess::getInfo()
{
    DataPacket pack;
    pack.nCMD = COM_CMD_INFO;
    pack.nLength = 0;
    sendPackAndStartRetry(pack);
}

