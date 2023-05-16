#include "1602A.h"
#include "stm32f1xx_hal.h"

void LCD_1602A_WriteChar(LCD_1602A* LCD, uint8_t Data, uint8_t Mode) {
	uint8_t Data_H;
	uint8_t Data_L;
	Data_H = Data & 0xF0;
	Data_L = (Data<<4) & 0xF0;
	Data_H |= CTRL_BL_DEFAULT|Mode; 
	Data_L |= CTRL_BL_DEFAULT|Mode; 
	
	Data_H |= CTRL_EN;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->address, (uint8_t*)&Data_H, 1, 1000);
	Data_H &= ~CTRL_EN;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->address, (uint8_t*)&Data_H, 1, 1000);
	Data_L |= CTRL_EN;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->address, (uint8_t*)&Data_L, 1, 1000);
	Data_L &= ~CTRL_EN;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->address, (uint8_t*)&Data_L, 1, 1000);
}

void LCD_1602A_WriteString(LCD_1602A* lcd, char* string) {
		while(*string) LCD_1602A_WriteChar(lcd, *string++, CTRL_DATA_MODE);
}

void LCD_1602A_SetCursor(LCD_1602A* lcd, uint8_t row, uint8_t col) {
		if(row >= LCD_ROWS || col >= LCD_COLS) {
				return;
		}
		uint8_t ddram_addr = row * 0x40 + col;
		LCD_1602A_WriteChar(lcd, LCD_CM_DDRAM_ADSET|ddram_addr, CTRL_COMMAND_MODE);
}

void LCD_1602A_Init(LCD_1602A* lcd, I2C_HandleTypeDef* hi2c, uint8_t address) {
		lcd->I2C = hi2c;
		lcd->address = address;
		
		//initialization process
		HAL_Delay(16);
		LCD_1602A_WriteChar(lcd, 0x30, CTRL_COMMAND_MODE);
		HAL_Delay(5);
		LCD_1602A_WriteChar(lcd, 0x30, CTRL_COMMAND_MODE);
		HAL_Delay(1);
		LCD_1602A_WriteChar(lcd, 0x32, CTRL_COMMAND_MODE);
		LCD_1602A_WriteChar(lcd, 0x20, CTRL_COMMAND_MODE);

		LCD_1602A_WriteChar(lcd, LCD_CM_FUNCTIONSET_DEFAULT, CTRL_COMMAND_MODE);
		//LCD_1602A_WriteChar(lcd, 0x80, CTRL_COMMAND_MODE); // display off
		LCD_1602A_WriteChar(lcd, LCD_CM_INPUTSET_DEFAULT, CTRL_COMMAND_MODE); //4DL, 2N, 5x8s
		LCD_1602A_WriteChar(lcd, LCD_CM_DISPLAYSWITCH_DEFAULT, CTRL_COMMAND_MODE); // display on, no cursor, no blink
		LCD_1602A_WriteChar(lcd, LCD_CM_CLEAR, CTRL_COMMAND_MODE); // display clear	
		HAL_Delay(2); //need up yo 1.64s to clear screen
}

void LCD_1602A_ClearScrean(LCD_1602A* lcd) {
		LCD_1602A_WriteChar(lcd, LCD_CM_CLEAR, CTRL_COMMAND_MODE);
		HAL_Delay(2); //need up yo 1.64s to clear screen
}

