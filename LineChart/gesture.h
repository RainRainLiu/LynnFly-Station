#ifndef GESTURE_H
#define GESTURE_H

#include <QObject>
#include "Componemts/communicationFormat.h"

class gesture : public communicationFormat
{
    Q_OBJECT
public:
    explicit gesture(QObject *parent = nullptr);

    typedef enum
    {
        GESTURE_GYRO_X,
        GESTURE_GYRO_Y,
        GESTURE_GYRO_Z,
        GESTURE_ACC_X,
        GESTURE_ACC_Y,
        GESTURE_ACC_Z,
        GESTURE_ANGLE_X,
        GESTURE_ANGLE_Y,
        GESTURE_ANGLE_Z,
    }GESTURE_TYPE_T;


private:
    void PackProcess(DataPacket *packet);

signals:
    void GestureUpdate(GESTURE_TYPE_T gestureType, uint32_t data);

public slots:


};

#endif // GESTURE_H
