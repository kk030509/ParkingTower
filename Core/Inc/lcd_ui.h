/*
 * lcd_ui.h
 *
 *  Created on: Jul 26, 2026
 *      Author: gaeun
 */

#ifndef INC_LCD_UI_H_
#define INC_LCD_UI_H_
#include "main.h"

void lcdShowIdle(void);

void lcdShowInQuestion(int slot);
void lcdShowInComplete(int slot);
void lcdShowInCancel(void);

void lcdShowOutQuestion(int slot);
void lcdShowMoving(int slot);
void lcdShowOutCancel(void);

void lcdShowThankYou(uint32_t durationSec);    // 출차 완료 후 3초 표시

#endif /* INC_LCD_UI_H_ */
