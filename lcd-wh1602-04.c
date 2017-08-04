//************************************************************************//
//	LCD HD44780  LCD-wh1602  LCD-wh1604

//	Alex_EXE 
//	http://alex-exe.ru/category/radio/stm32/stm32-lcd-hd44780-spl

//  Alexander Komarov
//  Add Russian language and fix bugs for 1604
//  https://bitbucket.org/ignusius/lcd-wh1602-04-stm32f10x

//************************************************************************//
#include "lcd-wh1602-04.h"

const unsigned char lcd44780_addLUT[4] = {0x80, 0xC0, 0x90, 0xD0};
unsigned char lcd44780_Address, lcd44780_Line;
const unsigned char russian[]={ 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45,
0xA3, 0xA4, 0xA5,0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50,0x43,
0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD,0xAE, 0x62,
0xAF, 0xB0, 0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63,0xBF,
0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2,0xC3, 0xC4, 0xC5,
0xC6, 0xC7 };

//************************************************************************//
GPIO_InitTypeDef lcd44780_GPIO_InitStructure;


void lcd44780_delay(unsigned int p)
{
	unsigned long i;
	for(i=0;i<(p*10);i++){}
}


void lcd44780_init_pins(void)
{
	RCC_APB2PeriphClockCmd(lcd44780_RCC, ENABLE);
	lcd44780_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	lcd44780_GPIO_InitStructure.GPIO_Pin = 	lcd44780_pin_E | lcd44780_pin_RS | lcd44780_pins_data;
	lcd44780_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(lcd44780_port, &lcd44780_GPIO_InitStructure);
}
//************************************************************************//

void lcd44780_WriteNibble(unsigned char data)
{
	GPIO_SetBits(lcd44780_port, ((data & 0x0F))<<lcd44780_offset);
	lcd44780_delay(200);
	lcd44780_E_1;
	lcd44780_delay(100);
	lcd44780_E_0;
	GPIO_ResetBits(lcd44780_port, 0x0F<<lcd44780_offset);
}


void lcd44780_WriteByte(unsigned char data)
{
	lcd44780_WriteNibble(data >> 4);
	lcd44780_WriteNibble(data & 0x0F);
}


void lcd44780_GoToLine(char LineNum)
{
	lcd44780_RS_0;
	lcd44780_Address = lcd44780_addLUT[LineNum-1];
	lcd44780_WriteByte(lcd44780_Address);
	lcd44780_RS_1;
	lcd44780_Address = 0;
	lcd44780_Line = LineNum;
}


void lcd44780_ClearLCD(void)
{
	lcd44780_RS_0;
	lcd44780_WriteByte(0x01);
	lcd44780_delay(1000);
	lcd44780_RS_1;
	lcd44780_GoToLine(1);
}


void lcd44780_SetLCDPosition(char x, char y)
{
	lcd44780_RS_0;
	lcd44780_Address = lcd44780_addLUT[y] + x;
	lcd44780_WriteByte(lcd44780_Address);
	lcd44780_RS_1;
	lcd44780_Line = y+1;
}


void lcd44780_ShowChar(unsigned char c)
{

	lcd44780_RS_1;
	if(c>=192) lcd44780_WriteByte(russian[c-192]);
	else lcd44780_WriteByte(c);
	lcd44780_Address++;
	switch (lcd44780_Address)
	{
		case 20: lcd44780_GoToLine(2); break;
		case 40: lcd44780_GoToLine(3); break;
		case 60: lcd44780_GoToLine(4); break;
		case 80: lcd44780_GoToLine(1); break;
	}
}


void lcd44780_ShowStr(unsigned char *s)
{
	while (*s != 0) lcd44780_ShowChar(*s++);
}

void lcd44780_init(void)
{
	unsigned char i;
	lcd44780_E_0;
	lcd44780_RS_0;
	lcd44780_delay(5000);
	lcd44780_WriteNibble(0x33);
	lcd44780_WriteNibble(0x33);
	lcd44780_WriteNibble(0x33);
	lcd44780_WriteNibble(0x22);
	lcd44780_WriteByte(0x28);
	lcd44780_WriteByte(0x01);
	lcd44780_WriteByte(0x10);
	lcd44780_WriteByte(0x06);
	lcd44780_WriteByte(0x0C);
	for(i=0x40; i<0x5F; i++)
	{
		lcd44780_delay(1000);
		lcd44780_RS_0;
		lcd44780_WriteByte(i);
		lcd44780_delay(1000);
		lcd44780_ShowChar(0);
	}
	lcd44780_RS_1;
	lcd44780_ClearLCD();
}
