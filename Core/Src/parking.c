#include "parking.h"
#include <string.h>

#define PARKING_CNT 8

static ParkingInfo parkingTable[PARKING_CNT] =
{
    {{0,0,0,0,0},0,0},
    {{0,0,0,0,0},1,0},
    {{0,0,0,0,0},2,0},
    {{0,0,0,0,0},3,0},
    {{0,0,0,0,0},4,0},
    {{0,0,0,0,0},5,0},
    {{0,0,0,0,0},6,0},
	{{0,0,0,0,0},7,0}
};
int assignCar(uint8_t *uid)
{
    int slot = findEmptySlot();

    if(slot == -1)
        return -1;

    memcpy(parkingTable[slot].uid, uid, 5);
    parkingTable[slot].parked = 1;

    return slot;
}
void removeCar(int idx)
{
    memset(parkingTable[idx].uid, 0, 5);
    parkingTable[idx].parked = 0;
}
int findCar(uint8_t *uid)
{
    for(int i = 0; i < PARKING_CNT; i++)
    {
        if(parkingTable[i].parked &&
           memcmp(uid, parkingTable[i].uid, 5) == 0)
        {
            return i;
        }
    }

    return -1;
}

uint8_t getSlot(int idx)
{
    return parkingTable[idx].slot;
}

uint8_t isParked(int idx)
{
    return parkingTable[idx].parked;
}

int findEmptySlot(void)
{
    for(int i=0;i<PARKING_CNT;i++)
    {
        if(parkingTable[i].parked == 0)
        {
            return i;
        }
    }

    return -1;
}

int getEmptyCount(void)
{
    int cnt = 0;

    for(int i = 0; i < PARKING_CNT; i++)
    {
        if(parkingTable[i].parked == 0)
            cnt++;
    }

    return cnt;
}
