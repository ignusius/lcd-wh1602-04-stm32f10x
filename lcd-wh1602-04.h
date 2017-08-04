#ifndef LCD-WH1602-04_H
#define LCD-WH1602-04_H

#include "stm32f10x.h" 
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define lcd44780_port					GPIOB
#define lcd44780_pin_RS				GPIO_Pin_10
#define lcd44780_pin_E				GPIO_Pin_11
#define lcd44780_pins_data		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define lcd44780_RCC					RCC_APB2Periph_GPIOB
#define lcd44780_offset 			12

#define lcd44780_RS_1 GPIO_SetBits(lcd44780_port, lcd44780_pin_RS);
#define lcd44780_E_1  GPIO_SetBits(lcd44780_port, lcd44780_pin_E);
#define lcd44780_RS_0 GPIO_ResetBits(lcd44780_port, lcd44780_pin_RS);
#define lcd44780_E_0  GPIO_ResetBits(lcd44780_port, lcd44780_pin_E);
void lcd44780_delay(unsigned int p);

void lcd44780_ClearLCD(void);

void lcd44780_SetLCDPosition(char x, char y);

void lcd44780_ShowChar(unsigned char c);

void lcd44780_ShowStr(unsigned char *s);

void lcd44780_init_pins(void);

void lcd44780_init(void);

#endif /*  LCD-WH1602-04_H */
