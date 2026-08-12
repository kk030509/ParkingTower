#include "tower.h"
#include "stepper.h"

uint8_t currentSlot = 0;

void moveToSlot(uint8_t targetSlot)
{
    if(targetSlot >= TOTAL_SLOTS)
        return;

    if(targetSlot == currentSlot)
        return;

    uint8_t cw  = (targetSlot - currentSlot + TOTAL_SLOTS) % TOTAL_SLOTS;
    uint8_t ccw = (currentSlot - targetSlot + TOTAL_SLOTS) % TOTAL_SLOTS;

    if(cw <= ccw)
    {
        for(uint8_t i = 0; i < cw; i++)
        {
            moveOneSlot(DIR_CW);
        }
    }
    else
    {
        for(uint8_t i = 0; i < ccw; i++)
        {
            moveOneSlot(DIR_CCW);
        }
    }
}
