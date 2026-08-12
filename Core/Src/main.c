/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RC522.h"
#include "string.h"
#include "stdio.h"
#include "stepper.h"
#include "tower.h"
#include "button.h"
#include "parking.h"
#include "led.h"
#include "lcd_ui.h"
#include "HD44780_LCD.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t status;
uint8_t str[16];
uint8_t sNum[5];
uint8_t slot = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t status;
uint8_t str[16];
uint8_t sNum[5];
uint32_t slotEntryTick[8] = {0};
typedef enum
{
    APP_IDLE,

    APP_CONFIRM_IN,
    APP_MOVE_IN,
    APP_IN_COMPLETE,

    APP_CONFIRM_OUT,
    APP_MOVE_OUT,
    APP_OUT_COMPLETE

} AppState;
AppState appState = APP_IDLE;
int outIdx = -1;
int targetSlot = -1;
uint32_t stateTick = 0;
uint32_t durationSec = 0;

uint8_t status;
uint8_t str[16];
uint8_t sNum[5];
HD44780_LCD_t lcd;
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  MFRC522_Init();
  LCD_createI2C(&lcd, &hi2c1);
   LCD_init(&lcd);
   LCD_enableBacklight(&lcd);
   lcdShowIdle();
  uint8_t version = Read_MFRC522(VersionReg);

  char buf[50];
  sprintf(buf, "Version = 0x%02X\r\n", version);
  HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(appState)
	     {
	     //--------------------------------------------------
	     // RFID 대기
	     //--------------------------------------------------
	     case APP_IDLE:

	         lcdShowIdle();

	         status = MFRC522_Request(PICC_REQIDL, str);

	         if(status == MI_OK)
	         {
	             status = MFRC522_Anticoll(str);

	             if(status == MI_OK)
	             {
	                 memcpy(sNum, str, 5);

	                 printf("\r\nUID : %02X %02X %02X %02X\r\n",
	                         sNum[0], sNum[1], sNum[2], sNum[3]);

	                 int idx = findCar(sNum);

	                 if(idx == -1)
	                 {
	                     targetSlot = findEmptySlot();

	                     lcdShowInQuestion(targetSlot);

	                     printf("In ? (YES / NO)\r\n");

	                     appState = APP_CONFIRM_IN;
	                 }
	                 else
	                 {
	                     outIdx = idx;
	                     targetSlot = getSlot(idx);

	                     lcdShowOutQuestion(targetSlot);

	                     printf("Out ? (YES / NO)\r\n");

	                     appState = APP_CONFIRM_OUT;
	                 }

	                 while(MFRC522_Request(PICC_REQIDL, str) == MI_OK)
	                 {
	                     HAL_Delay(100);
	                 }
	             }
	         }

	         break;

	     //--------------------------------------------------
	     // 입차 확인
	     //--------------------------------------------------
	     case APP_CONFIRM_IN:

	         if(buttonYesPressed())
	         {
	             printf("YES\r\n");

	             targetSlot = assignCar(sNum);

	             if(targetSlot == -1)
	             {
	                 printf("Parking Full!!\r\n");

	                 appState = APP_IDLE;
	             }
	             else
	             {
	                 appState = APP_MOVE_IN;
	             }
	         }
	         else if(buttonNoPressed())
	         {
	             lcdShowInCancel();

	             HAL_Delay(2000);

	             appState = APP_IDLE;
	         }

	         break;

	     //--------------------------------------------------
	     // 입차 이동
	     //--------------------------------------------------
	     case APP_MOVE_IN:

	         lcdShowMoving(targetSlot);

	         ledUpdate();

	         printf("Move -> Slot %d\r\n", targetSlot + 1);

	         moveToSlot(targetSlot);

	         slotEntryTick[targetSlot] = HAL_GetTick();

	         stateTick = HAL_GetTick();

	         appState = APP_IN_COMPLETE;

	         break;

	     //--------------------------------------------------
	     // 입차 완료
	     //--------------------------------------------------
	     case APP_IN_COMPLETE:

	         lcdShowInComplete(targetSlot);

	         printf("Arrived Slot %d\r\n", targetSlot + 1);

	         if(HAL_GetTick() - stateTick >= 2000)
	         {
	             appState = APP_IDLE;
	         }

	         break;

	     //--------------------------------------------------
	     // 출차 확인
	     //--------------------------------------------------
	     case APP_CONFIRM_OUT:

	         if(buttonYesPressed())
	         {
	             printf("YES\r\n");

	             appState = APP_MOVE_OUT;
	         }
	         else if(buttonNoPressed())
	         {
	             lcdShowOutCancel();

	             HAL_Delay(2000);

	             appState = APP_IDLE;
	         }

	         break;

	     //--------------------------------------------------
	     // 출차 이동
	     //--------------------------------------------------
	     case APP_MOVE_OUT:

	         lcdShowMoving(targetSlot);

	         printf("Move -> Slot %d\r\n", targetSlot + 1);

	         moveToSlot(targetSlot);

	         durationSec =
	                 (HAL_GetTick() - slotEntryTick[targetSlot]) / 1000;

	         removeCar(outIdx);

	         ledUpdate();

	         stateTick = HAL_GetTick();

	         appState = APP_OUT_COMPLETE;

	         break;

	     //--------------------------------------------------
	     // 출차 완료
	     //--------------------------------------------------
	     case APP_OUT_COMPLETE:

	         lcdShowThankYou(durationSec);

	         printf("Car Out Complete\r\n");

	         if(HAL_GetTick() - stateTick >= 3000)
	         {
	             appState = APP_IDLE;
	         }

	         break;
	     }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
