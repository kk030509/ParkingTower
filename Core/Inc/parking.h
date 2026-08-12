#ifndef INC_PARKING_H_
#define INC_PARKING_H_

#include "main.h"

typedef struct
{
    uint8_t uid[5];
    uint8_t slot;
    uint8_t parked;
} ParkingInfo;

// UID로 차량 검색 (있으면 슬롯 인덱스 반환, 없으면 -1)
int findCar(uint8_t *uid);
// 빈 슬롯 검색 (없으면 -1)
int findEmptySlot(void);
// 차량 입차
// 빈 슬롯에 UID 저장 후 슬롯 인덱스 반환
int assignCar(uint8_t *uid);
// 차량 출차
// UID 삭제 및 슬롯 비우기
void removeCar(int idx);
// 슬롯 번호 반환
uint8_t getSlot(int idx);
// 주차 여부 반환
uint8_t isParked(int idx);
//빈자리 개수 찾기
int getEmptyCount(void);
#endif /* INC_PARKING_H_ */
