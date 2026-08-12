#include "lcd_ui.h"
#include "HD44780_LCD.h"   // LCD 라이브러리
#include "parking.h"
#include <stdio.h>

extern HD44780_LCD_t lcd;
extern uint32_t duratinSec;


void lcdShowIdle(void)
{
	char buf[17];

	    LCD_clearDisplay(&lcd);
	    HAL_Delay(4);

	    LCD_setCursorPos(&lcd,0,0);
	    LCD_Print(&lcd,"Parking Tower");

	    sprintf(buf,"Empty : %d",getEmptyCount());

	    LCD_setCursorPos(&lcd,1,0);
	    LCD_Print(&lcd,buf);
}

void lcdShowInQuestion(int slot)
{
	char buf[17];

	    LCD_clearDisplay(&lcd);
	    HAL_Delay(4);

	    LCD_setCursorPos(&lcd,0,0);
	    LCD_Print(&lcd,"Parking In?");

	    sprintf(buf,"Slot #%d  Y/N",slot+1);

	    LCD_setCursorPos(&lcd,1,0);
	    LCD_Print(&lcd,buf);
}

void lcdShowInComplete(int slot)
{
	char buf[17];

	    LCD_clearDisplay(&lcd);
	    HAL_Delay(4);

	    LCD_setCursorPos(&lcd,0,0);
	    LCD_Print(&lcd,"Complete");

	    sprintf(buf,"Slot #%d In",slot+1);

	    LCD_setCursorPos(&lcd,1,0);
	    LCD_Print(&lcd,buf);
}

void lcdShowInCancel(void)
{
	LCD_clearDisplay(&lcd);
	HAL_Delay(4);

	LCD_setCursorPos(&lcd, 0, 0);
	LCD_Print(&lcd, "Parking");

	LCD_setCursorPos(&lcd, 1, 0);
	LCD_Print(&lcd, "Canceled");
}

void lcdShowOutQuestion(int slot)
{
	char buf[17];

	    LCD_clearDisplay(&lcd);
	    HAL_Delay(4);

	    LCD_setCursorPos(&lcd,0,0);
	    LCD_Print(&lcd,"Parking Out?");

	    sprintf(buf,"Slot #%d  Y/N",slot+1);

	    LCD_setCursorPos(&lcd,1,0);
	    LCD_Print(&lcd,buf);
}

void lcdShowMoving(int slot)
{
    char buf[17];

    LCD_clearDisplay(&lcd);
    HAL_Delay(4);

    LCD_setCursorPos(&lcd, 0, 0);
    LCD_Print(&lcd, "Moving...");

    sprintf(buf, "Slot #%d", slot+1);
    LCD_setCursorPos(&lcd, 1, 0);
    LCD_Print(&lcd, buf);
}

void lcdShowOutCancel(void)
{
	LCD_clearDisplay(&lcd);
	HAL_Delay(4);

	LCD_setCursorPos(&lcd, 0, 0);
	LCD_Print(&lcd, "Exit");

	LCD_setCursorPos(&lcd, 1, 0);
	LCD_Print(&lcd, "Canceled");
}

void lcdShowThankYou(uint32_t durationSec)
{
	 char buf[17];

	    uint32_t min = durationSec/60;
	    uint32_t sec = durationSec%60;

	    LCD_clearDisplay(&lcd);
	    HAL_Delay(4);

	    LCD_setCursorPos(&lcd,0,0);
	    LCD_Print(&lcd,"Thank You");

	    sprintf(buf,"Time %02lu:%02lu",
	            (unsigned long)min,
	            (unsigned long)sec);

	    LCD_setCursorPos(&lcd,1,0);
	    LCD_Print(&lcd,buf);
}
