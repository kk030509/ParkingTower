

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"
#define DEBOUNCE_DELAY 50

#define YES_PORT    GPIOC
#define YES_PIN     GPIO_PIN_8

#define NO_PORT     GPIOC
#define NO_PIN      GPIO_PIN_9

void buttonInit(void);
uint8_t buttonYesPressed(void);
uint8_t buttonNoPressed(void);



#endif /* INC_BUTTON_H_ */
