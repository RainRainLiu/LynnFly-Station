#include "format.h"

format::format(QObject *parent) : QObject(parent)
{

}
/******************************************
 * @函数说明：选择端口的信号槽
 * @输入参数：QAction * action 读取action->text
 *          作为端口号的名称
 * @返回参数：无
 * @修订日期：
******************************************/
void format::PacketParsing(uint8_t data)
{
    switch(step)
    {
        case 0:
        {
            if (data == 0xeb)                    //header check
            {
                step++;
            }
        }
        break;
        case 1:
        {
            if (data == 0x90)                    //header check
            {
                step++;
            }
            else
            {
                ReceiveError();
            }
        }
        break;

        case 2:
        {
            CMD = data;              //save cmd
            step++;
        }
        break;

        case 3:
        {
            if (data == 0)
            {
                step = 0x21;         //no data

            }
            else
            {
                step = 0x20;         // receive data
                length = data;
            }
            check_sum = 0;
            count = 0;
        }
        break;

        case 0x20:
            Data[count] = data;
            check_sum += data;
            count++;
            if (count >= length)       //data receive carry out
            {
                step = 0x21;
            }
            break;

        case 0x21:
            if ((check_sum & 0xFF) == data)           //check sum hight bit
            {
                step = 0x22;
            }
            else
            {
                ReceiveError();
            }
            break;


        case 0x22:
            if( (check_sum >> 8) == data)       //check sum low bit
            {
                step = 0xff;
            }
            else
            {
                ReceiveError();
            }
            break;


        case 0xff:
            if (data == 0x03)                                // tail check
            {
                step = 0;
            }
            else
            {
                ReceiveError();
            }
            break;
    }
}

/******************************************
 * @函数说明：选择端口的信号槽
 * @输入参数：QAction * action 读取action->text
 *          作为端口号的名称
 * @返回参数：无
 * @修订日期：
******************************************/
void format::ReceiveError()
{
    step = 0;
    ErrorNumber = step;
}
