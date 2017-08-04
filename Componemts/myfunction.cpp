#include "Componemts/communicationFormat.h"
#include "stdint.h"

uint32_t MyFuncation_byteToUint32(uint8_t *byte)
{
    uint32_t result = byte[0];

    result |= (byte[1] << 8);
    result |= (byte[2] << 16);
    result |= (byte[3] << 24);

    return result;
}
