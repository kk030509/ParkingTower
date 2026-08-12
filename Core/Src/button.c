#include "button.h"

#define YES_PORT    GPIOC
#define YES_PIN     GPIO_PIN_8

#define NO_PORT     GPIOC
#define NO_PIN      GPIO_PIN_9

static uint32_t yesTime = 0;
static uint32_t noTime = 0;

static GPIO_PinState lastYesState = GPIO_PIN_SET;
static GPIO_PinState lastNoState  = GPIO_PIN_SET;

void buttonInit(void)
{
    lastYesState = HAL_GPIO_ReadPin(YES_PORT, YES_PIN);
    lastNoState  = HAL_GPIO_ReadPin(NO_PORT, NO_PIN);
}

uint8_t buttonYesPressed(void)
{
    GPIO_PinState current = HAL_GPIO_ReadPin(YES_PORT, YES_PIN);

    if(current != lastYesState)
    {
        if(HAL_GetTick() - yesTime > DEBOUNCE_DELAY)
        {
            yesTime = HAL_GetTick();
            lastYesState = current;

            if(current == GPIO_PIN_RESET)
                return 1;
        }
    }

    lastYesState = current;

    return 0;
}

uint8_t buttonNoPressed(void)
{
    GPIO_PinState current = HAL_GPIO_ReadPin(NO_PORT, NO_PIN);

    if(current != lastNoState)
    {
        if(HAL_GetTick() - noTime > DEBOUNCE_DELAY)
        {
            noTime = HAL_GetTick();
            lastNoState = current;

            if(current == GPIO_PIN_RESET)
                return 1;
        }
    }

    lastNoState = current;

    return 0;
}
