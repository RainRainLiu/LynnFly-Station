#include "deceivemontor.h"

deceiveMontor::deceiveMontor(QObject *parent, mySerialPort *serial) : QObject(parent)
{
    serialPort = serial;
}


