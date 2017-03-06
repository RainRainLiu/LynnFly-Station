#include "format.h"
const uint8_t   STX = 0xeb;
const uint8_t   ETX = 0x30;
const uint8_t   ESC = 0xef;

format::format(QObject *parent) : QObject(parent)
{
}
/******************************************************************
  * @ Name          : void ReceiveError(DataPack_typdef *data_pack)
  * @ Description   : receive error, reset step, save error number
  * @ Input         : DataPack_typdef *data_pack
  * @ Output        : None
  * @ Return        : None
  * @ Modify the record : ---
******************************************************************/
void format::ReceiveError()
{
    ErrorNumber = step;
    step = 0;
}

uint8_t CalculateCheckSum(DataPacket *pPack)
{
    uint8_t checkSum = STX;

    checkSum += pPack->nCMD;
    checkSum += (uint8_t)(pPack->nLength & 0xff);
    checkSum += (uint8_t)((pPack->nLength >> 8) & 0xff);

    uint32_t i;

    for (i = 0; i < pPack->nLength; i ++)
    {
        checkSum += pPack->aData[i];
    }

    return checkSum;
}
/************************
 * 转义处理
************************/
static uint16_t DisposeESC(uint32_t nLength, uint8_t *pOut, const uint8_t *pIn)
{
    uint32_t i = 0;
    uint16_t temp;
    uint16_t out_length = 0;
    for (i = 0; i < nLength; i++)
    {
        if (*pIn == STX || *pIn  == ETX || *pIn  == ESC)  //need to  escape data processing
        {
            temp = *pIn | 0x100;
            *pOut = ESC;
            out_length++;
            pOut++;
            *pOut = (uint8_t)((temp - ESC) & 0XFF);
        }
        else
        {
            *pOut = *pIn;

        }
        out_length++;
        pOut++;
        pIn++;
    }
    return out_length;
}

/******************************************************************
  * @ Name          : void PacketParsing(DataPack_typdef *data_pack, uint8_t data)
  * @ Description   : Packet parsing
  * @ Input         : DataPack_typdef *data_pack
  * @ Output        : None
  * @ Return        : None
  * @ Modify the record : ---
******************************************************************/
DataPacket* format::Parsing(uint8_t inData)
{
    uint8_t data;

    if(inData == STX)     // find STX
    {
        data = inData;
        ReceiveError();
    }
    else if(inData == ESC && lastData != ESC)
    {
        lastData = inData;
        return NULL;
    }
    else if(lastData == ESC)
    {
        data = (uint8_t)((inData + ESC) & 0xFF);
        lastData= 0;
    }
    else
    {
        data = inData;
    }

    switch(step)
    {
        case 0:
        {
            if (data == STX)                    //header check
            {
                step++;
            }
        }
        break;

        case 1:
        {
            packet.nCMD = data;
            step++;
            count = 0;
        }
        break;

        case 2:
        {
            if (count == 0)
            {
                packet.nLength = data;
                count++;
            }
            else
            {
                packet.nLength |= (data >> 8);
                step++;
                count = 0;
                if (packet.nLength == 0)
                {
                    step++;
                }
            }
        }
        break;

        case 3:
        {
            packet.aData[count] = data;

            count++;
            if (count >= packet.nLength)       //data receive carry out
            {
                step++;
            }
        }
        break;

        case 4:
        {
            if (CalculateCheckSum(&packet) == data)
            {
                step++;
            }
            else
            {
                ReceiveError();
            }
        }
        break;

        case 5:
        {
            if (data == ETX)
            {
                return &packet;
            }
        }
        break;
    }
    return NULL;
}

/******************************************************************
  * @ Name          : void ReceiveError(DataPack_typdef *pParsing)
  * @ Description   : receive error, reset step, save error number
  * @ Input         : DataPack_typdef *pParsing
  * @ Output        : None
  * @ Return        : None
  * @ Modify the record : ---
*****************************************************************/
QByteArray format::BuildPack(DataPacket *pPack)
{
    if (pPack->nLength > PACKET_DATA_LENGTH)
    {
        return NULL;
    }

    QByteArray array;
    uint8_t *escBuf = (uint8_t *)malloc((PACKET_DATA_LENGTH + 6) * 2);//[(PACKET_DATA_LENGTH + 6) * 2];
    uint32_t length = 0;


    escBuf[length++] = STX;
    length += DisposeESC(1, &escBuf[length], &pPack->nCMD);
    length += DisposeESC(2, &escBuf[length], (uint8_t *)&pPack->nLength);
    length += DisposeESC(pPack->nLength, &escBuf[length], (uint8_t *)&pPack->aData);


    escBuf[length++] = CalculateCheckSum(pPack);
    escBuf[length++] = ETX;

    array.append((const char*)escBuf, (int)length);

    return array;
}


