#pragma once
#include "stdafx.h"
#include "Fase1.h"
#include "BasicLCD.h"

#define MAX_LENGTH 32
#define END_OF_LINE 16


class Fase2 : public BasicLCD
{
	public:
	 
		Fase2();
		~Fase2();
	
		virtual bool lcdInitOk();
	
		virtual FT_STATUS lcdGetError();
	
		virtual bool lcdClear();
		virtual bool lcdClearToEOL();
	
		virtual Fase2& operator<<(const unsigned char c);
		virtual Fase2& operator<<(const unsigned char * c);	
	
		virtual bool lcdMoveCursorUp();
		virtual bool lcdMoveCursorDown();	
		virtual bool lcdMoveCursorRight();
		virtual bool lcdMoveCursorLeft();
		virtual bool lcdSetCursorPosition(const cursorPosition pos);	
		virtual cursorPosition lcdGetCursorPosition();

	protected:
		int cadd;
		void lcdUpdateCursor();

	private:
		Fase1 * fase1;
		bool error;
		bool getError(); // true si hubo error 
};