#ifndef DI_ACQ_H
#define DI_ACQ_H
#include <stdint.h>

typedef union
{
	uint8_t all;
	struct
	{
		uint8_t stCh1UB : 1; // 通道1
		uint8_t stCh2UB : 1; // 通道2
		uint8_t stCh3UB : 1; // 通道3
		uint8_t stCh4UB : 1; // 通道4
		uint8_t stCh5UB : 1; // 通道5
	} bit;
} Di_infoDisIn_TYPE;

void Di_Poll(void);
void Di_DataAcq(uint8_t *data);
extern Di_infoDisIn_TYPE Di_infoDisIn;

#endif
