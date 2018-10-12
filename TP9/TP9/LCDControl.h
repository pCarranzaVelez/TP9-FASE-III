#pragma once

#define PORT_P0 0
#define PORT_P1 1

#define LCD_RS			(1 << PORT_P1) //0x02. Prende el RS, apaga todo lo demas.
#define LCD_E			(1 << PORT_P0) //0x01. Prende el E, apaga todo lo demas.
#define LCD_RS_ON		LCD_RS
#define LCD_RS_OFF		(LCD_RS ^ LCD_RS)
#define MSN(x)			((x) & 0xF0)
#define LSN(x)			( ( (x) << 4 ) & 0xF0)

#define FONT_BIT					(2) //USED IN FUNCTION SET
#define LINE_BIT					(3) //USED IN FUNCTION SET
#define DL_BIT						(4) //USED IN FUNCTION SET

//Funciones
#define MODE_8BIT					(1<<DL_BIT)
#define MODE_4BIT					(0<<DL_BIT)// 0 EL BIT DL
#define CLEAR_SCREEN				0x01
#define RETURN_HOME					0x02
#define ENTRY_MODE_SET				0x06
#define DISPLAY_CONTROL				0x08

#define FUNCTION_SET				0x20	
#define DSP_LINES_ONE				(0<<LINE_BIT) //USED IN FUNCTION SET
#define DSP_LINES_TWO				(1<<LINE_BIT) //USED IN FUNCTION SET
#define FONT_5X8					(0<<FONT_BIT) //USED IN FUNCTION SET
#define FONT_5X10					(1<<FONT_BIT) //USED IN FUNCTION SET

#define DISPLAY_ON_OFF_CONTROL		0x0F	
#define SET_ADDRESS(a,b)			(0x80 + (a) + (b) * 0x40) // 0x80 es SET_ADRESS (b = 0 en primera linea o b = 1 en segunda linea)