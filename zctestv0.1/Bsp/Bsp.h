#ifndef _BSP_H_
#define _BSP_H_

#include <stdint.h>
#include "spi.h"
#include "MySys.h"
#include "delay.h"

void Bsp_Init(void);

void Bsp_Poll(void);

#endif /* _BSP_H_ */
