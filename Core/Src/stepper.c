#include "stepper.h"
//덜 돌면 늘리셈, 더 돌면 줄이셈
#define STEPS_PER_SLOT 2048    // 나중에 보정
#include "tower.h"
#include "adc.h"
static uint32_t ignoreUntil = 0;
uint16_t readPhotoADC(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}
static const uint8_t HALF_STEP_SEQ[8][4] =
{
		{1,0,0,0},
		{1,1,0,0},
		{0,1,0,0},
		{0,1,1,0},
		{0,0,1,0},
		{0,0,1,1},
		{0,0,0,1},
		{1,0,0,1}
};

void stepMotor(uint8_t step)
{
	HAL_GPIO_WritePin(IN_GPIO_PORT, IN1_Pin, HALF_STEP_SEQ[step][0]);
	HAL_GPIO_WritePin(IN_GPIO_PORT, IN2_Pin, HALF_STEP_SEQ[step][1]);
	HAL_GPIO_WritePin(IN_GPIO_PORT, IN3_Pin, HALF_STEP_SEQ[step][2]);
	HAL_GPIO_WritePin(IN_GPIO_PORT, IN4_Pin, HALF_STEP_SEQ[step][3]);
}
void rotateSteps(uint16_t steps, uint8_t direction)
{
	for (uint16_t i =0;i<steps;i++)
	{
		uint8_t step;
		if(direction==DIR_CW)
		{
			step = i % 8;
		}
		else
		{
			step = 7-(i%8);
		}
		stepMotor(step);
		HAL_Delay(1);
	}
}
void rotateDegrees(uint16_t degree, uint8_t direction)
{
	uint16_t steps = (uint16_t)((uint32_t)(degree*STEPS_PER_REVOLUTION)/360);
	rotateSteps(steps, direction);
}
//한 슬롯 이동
void moveOneSlot(uint8_t direction)
{
	uint8_t step = 0;

	    // 출발 후 500ms 동안 센서 무시
	    ignoreUntil = HAL_GetTick() + 1500;

	    while(1)
	    {
	        if(direction == DIR_CW)
	            step = (step + 1) % 8;
	        else
	            step = (step + 7) % 8;

	        stepMotor(step);
	        HAL_Delay(1);

	        if(photoDetected())
	            break;
	    }

	    motorOff();

	    if(direction == DIR_CW)
	        currentSlot = (currentSlot + 1) % TOTAL_SLOTS;
	    else
	        currentSlot = (currentSlot + TOTAL_SLOTS - 1) % TOTAL_SLOTS;
}

void motorOff(void)
{
    HAL_GPIO_WritePin(IN_GPIO_PORT, IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_GPIO_PORT, IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_GPIO_PORT, IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_GPIO_PORT, IN4_Pin, GPIO_PIN_RESET);
}

uint8_t photoDetected(void)
{
    // 아직 무시 시간이라면 감지 안 함
    if(HAL_GetTick() < ignoreUntil)
        return 0;
    if(readPhotoADC() < 4000)
    {
        HAL_Delay(20);
        if(readPhotoADC() < 4000)
        {
            // 앞으로 2초 동안은 센서 무시 -> 이거 돌려보면서 시간 조절 필요
            ignoreUntil = HAL_GetTick() + 2000;
            return 1;
        }
    }
    return 0;
}
