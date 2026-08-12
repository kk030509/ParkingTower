#ifndef INC_TOWER_H_
#define INC_TOWER_H_

#include "main.h"

#define TOTAL_SLOTS 8

extern uint8_t currentSlot;

// 원하는 슬롯으로 이동
void moveToSlot(uint8_t targetSlot);

#endif /* INC_TOWER_H_ */
