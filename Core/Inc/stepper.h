#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"

#define STEPS_PER_REVOLUTION 4096

#define DIR_CW     0
#define DIR_CCW    1

#define IN_GPIO_PORT GPIOB

#define IN1_Pin GPIO_PIN_1
#define IN2_Pin GPIO_PIN_15
#define IN3_Pin GPIO_PIN_14
#define IN4_Pin GPIO_PIN_13

extern uint8_t currentSlot;

// Motor
void stepMotor(uint8_t step);
void motorOff(void);

// Movement
void rotateSteps(uint16_t steps, uint8_t direction);
void rotateDegrees(uint16_t degree, uint8_t direction);
void moveOneSlot(uint8_t direction);
void moveToSlot(uint8_t targetSlot);

// Photo sensor
uint16_t readPhotoADC(void);
uint8_t photoDetected(void);

#endif /* INC_STEPPER_H_ */
