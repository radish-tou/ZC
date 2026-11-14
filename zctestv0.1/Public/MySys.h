#ifndef MY_SYS_H
#define MY_SYS_H
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#define RTT_LOG_DEBUG // 开启RTT

#define FLASH_READ_PROTECTION_ENABLE // 开启FLASH读保护
/*=================引用文件=========================*/
#ifdef FLASH_READ_PROTECTION_ENABLE
// #include "FlashReadProtection.h"
#endif

// #include "RTT_LOG.h"

typedef uint32_t TickType_t;
#define MILLIS_TICK_MAX_VAL 0xFFFFFFFFUL
#define MICROS_TICK_MAX_VAL (0x003FFFFFUL * 1000UL + 1000UL)

/*--------------------------------- 空操作 ---------------------------------*/
#define NOP_1() __NOP() /* CMSIS standard NOP, works on all compilers */
#define NOP_2() \
    NOP_1();    \
    NOP_1()
#define NOP_3() \
    NOP_2();    \
    NOP_1()
#define NOP_4() \
    NOP_3();    \
    NOP_1()
#define NOP_5() \
    NOP_4();    \
    NOP_1()
#define NOP_6() \
    NOP_5();    \
    NOP_1()
#define NOP_7() \
    NOP_6();    \
    NOP_1()
#define NOP_8() \
    NOP_7();    \
    NOP_1()
#define NOP_9() \
    NOP_8();    \
    NOP_1()
#define NOP_10() \
    NOP_9();     \
    NOP_1()
#define NOP_20() \
    NOP_10();    \
    NOP_10()
#define NOP_30() \
    NOP_20();    \
    NOP_10()
#define NOP_40() \
    NOP_30();    \
    NOP_10()
#define NOP_50() \
    NOP_40();    \
    NOP_10()

__STATIC_INLINE void CPU_SoftwareReset(void)
{
    NVIC_SystemReset();
}

/*--------------------------------- 全局中断 ---------------------------------*/
__STATIC_INLINE void Enable_IRQ(void)
{
    __ASM volatile("cpsie i");
}

__STATIC_INLINE void Disable_IRQ(void)
{
    __ASM volatile("cpsid i");
}

extern volatile uint32_t irqNestingConuter;

__STATIC_INLINE void CPU_CriticalEnter(void)
{
    __ASM volatile("cpsid i"); /* 关全局中断 */
    irqNestingConuter++;
}

__STATIC_INLINE void CPU_CriticalExit(void)
{
    if (irqNestingConuter != 0)
    {
        irqNestingConuter--;
    }

    if (irqNestingConuter == 0)
    {
        __ASM volatile("cpsie i"); /* 使能全局中断 */
    }
}

/*----------------------------------- 临界区 ---------------------------------*/
__STATIC_INLINE uint32_t CPU_SR_Save(void)
{
    volatile uint32_t primask;

    primask = __get_PRIMASK();
    __disable_irq();

    return primask;
}

__STATIC_INLINE void CPU_SR_Restore(uint32_t primask)
{
    __set_PRIMASK(primask);
}

#define CPU_SR_ALLOC() uint32_t __cpuSR = 0 /* 定义变量 cpuSR */

#define CPU_INT_DIS() __cpuSR = CPU_SR_Save() /* Save    CPU status word & disable interrupts. */
#define CPU_INT_EN() CPU_SR_Restore(__cpuSR)  /* Restore CPU status word.                      */

// #define  CPU_CRITICAL_ENTER()   CPU_INT_DIS()           /* Disable   interrupts.                         */
// #define  CPU_CRITICAL_EXIT()    CPU_INT_EN()            /* Re-enable interrupts.                         */

#define CPU_CRITICAL_ENTER() CPU_CriticalEnter() /* Disable   interrupts.                         */
#define CPU_CRITICAL_EXIT() CPU_CriticalExit()   /* Re-enable interrupts.                         */

TickType_t Millis(void);
TickType_t Millis_Elapsed(TickType_t startTime);

#endif
