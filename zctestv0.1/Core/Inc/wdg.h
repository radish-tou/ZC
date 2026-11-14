#ifndef __WDG_H
#define __WDG_H

#include "stm32f4xx.h"
//#include "core_cm4.h"
//#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_iwdg.h"
#include "stm32f4xx_hal_wwdg.h"


void iwdg_init(uint32_t prer, uint16_t rlr);    /* 初始化IWDG，并使能IWDG */
void iwdg_feed(void);                           /* 喂狗 */

void wwdg_init(uint16_t tr, uint16_t wr, uint32_t fprer); /*窗口看门狗初始化*/
void wwdg_set_counter(uint8_t cnt);                     /*喂窗口看门狗*/
extern WWDG_HandleTypeDef wwdg_handler;  
#endif

