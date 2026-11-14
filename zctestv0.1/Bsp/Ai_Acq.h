#ifndef AI_ACQ_H
#define AI_ACQ_H
#include "can.h"

void TPC51701_Init(void);
void TPC51701_Poll(void);
void TPC51701_ReportAiData(CAN_HandleTypeDef *hcan, uint8_t *ai_data, uint8_t len);

#endif
