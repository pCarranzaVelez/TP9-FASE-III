#pragma once

#include "stdafx.h"

#define FTD_H
#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "LCDControl.h"

#define MY_LCD_DESCRIPTION "EDA LCD 2 B"

using namespace std;

class Fase1
{
	public:
	
		Fase1();
		~Fase1();
		bool lcdInit();
		bool lcdWriteIR(BYTE byte); 
		FT_STATUS changeFourBitMode();
		bool lcdWriteDR(BYTE byte);
		bool lcdWriteByte(BYTE value, BYTE rs);
		bool lcdWriteNibble(BYTE nibble);
		FT_STATUS getStatus();
		
	private:
		FT_HANDLE disp_handler;
		DWORD bytesSent;
		FT_STATUS disp_status;
};
