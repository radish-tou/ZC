#include "MySys.h"

volatile uint32_t irqNestingConuter = 0;

// 获取毫秒时间戳
inline TickType_t Millis(void)
{
    __IO uint32_t tick = HAL_GetTick();
    return tick;
}

inline TickType_t Millis_Elapsed(TickType_t startTime)
{
    __IO TickType_t curr = Millis();
    return (uint32_t)(curr - startTime); /* 溢出不影响 */
}
