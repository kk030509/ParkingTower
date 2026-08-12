#include "led.h"
#include "parking.h"

void ledUpdate(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,
			!isParked(0) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,
			!isParked(1) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,
			!isParked(2) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,
			!isParked(3) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,
			!isParked(4) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,
			!isParked(5) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,
			!isParked(6) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,
			!isParked(7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
