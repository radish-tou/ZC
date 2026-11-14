#include "gpio.h"
#include "Do_Ctrl.h"
#include "MySys.h"

void Do_OutCtl(uint8_t *data, uint8_t len)
{
    if (len != 8 || data == NULL)
        return;
    if (data[0] != 0x55 && data[1] != 0xAA)
        return;

    if (data[2] == 0x01)
        HAL_GPIO_WritePin(IO_OUT1_GPIO_Port, IO_OUT1_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT1_GPIO_Port, IO_OUT1_Pin, GPIO_PIN_SET);
    if (data[3] == 0x01)
        HAL_GPIO_WritePin(IO_OUT2_GPIO_Port, IO_OUT2_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT2_GPIO_Port, IO_OUT2_Pin, GPIO_PIN_SET);
    if (data[4] == 0x01)
        HAL_GPIO_WritePin(IO_OUT3_GPIO_Port, IO_OUT3_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT3_GPIO_Port, IO_OUT3_Pin, GPIO_PIN_SET);
    if (data[5] == 0x01)
        HAL_GPIO_WritePin(IO_OUT4_GPIO_Port, IO_OUT4_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT4_GPIO_Port, IO_OUT4_Pin, GPIO_PIN_SET);
    if (data[6] == 0x01)
        HAL_GPIO_WritePin(IO_OUT5_GPIO_Port, IO_OUT5_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT5_GPIO_Port, IO_OUT5_Pin, GPIO_PIN_SET);
    if (data[7] == 0x01)
        HAL_GPIO_WritePin(IO_OUT6_GPIO_Port, IO_OUT6_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(IO_OUT6_GPIO_Port, IO_OUT6_Pin, GPIO_PIN_SET);
}

// void Do_Poll(void)
// {
//     static TickType_t tick = 0;
//     if (Millis_Elapsed(tick) >= 20)
//     {
//         tick = Millis();
//         DisOutCtl();
//     }
// }