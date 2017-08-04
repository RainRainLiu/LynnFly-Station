#include "gesture.h"
#include "Componemts/myfunction.h"

gesture::gesture(QObject *parent) : communicationFormat(parent)
{

}


void gesture::PackProcess(DataPacket *packet)
{
    switch (packet->nCMD2) 
    {
        case COM_CMD2_GESTURE_OUTPUT_GYRO:
            emit GestureUpdate(GESTURE_GYRO_X, MyFuncation_byteToUint32(&packet->aData[0]));
            emit GestureUpdate(GESTURE_GYRO_Y, MyFuncation_byteToUint32(&packet->aData[4]));
            emit GestureUpdate(GESTURE_GYRO_Z, MyFuncation_byteToUint32(&packet->aData[8]));
            break;
        case COM_CMD2_GESTURE_OUTPUT_ACC:
            emit GestureUpdate(GESTURE_ACC_X, MyFuncation_byteToUint32(&packet->aData[0]));
            emit GestureUpdate(GESTURE_ACC_Y, MyFuncation_byteToUint32(&packet->aData[4]));
            emit GestureUpdate(GESTURE_ACC_Z, MyFuncation_byteToUint32(&packet->aData[8]));

            break;
        case COM_CMD2_GESTURE_OUTPUT_ANGLE:
            emit GestureUpdate(GESTURE_ANGLE_X, MyFuncation_byteToUint32(&packet->aData[0]));
            emit GestureUpdate(GESTURE_ANGLE_Y, MyFuncation_byteToUint32(&packet->aData[4]));
            emit GestureUpdate(GESTURE_ANGLE_Z, MyFuncation_byteToUint32(&packet->aData[8]));
            break;
        default:
            break;
    }

}
