#pragma once
#include "stdafx.h"
#include "Fase2.h"
#include <string>

using namespace std;
enum EVENT_TYPE { NO_EVENT, TIMER_EV, KB_EVENT, SOFTWARE_EV};

//estructura de los eventos
typedef struct
{
	EVENT_TYPE type;
	char data;
}event_t;


//estructura de los tweets
typedef struct
{
	std::string name;	
	std::string text;
	std::string time;
}tweetData_t;

class Dispatcher
{
	public:
		Dispatcher(tweetData_t * tweets, unsigned int tc, BasicLCD * lcd);	//constructor
		~Dispatcher();								//destructor
		void dispatch();							//el dispatcher en si
		bool getExit();								//getter del exit
		void setEvent(event_t input);				//setter del evento
		void setEvent(EVENT_TYPE type, char data);	//overload del setter del evento
		bool getTimeStatus();						//chequear si se cumplio el tick del timer
		bool checkLastTweet();						//chequear si es el ultimo tweet

	private:
		chrono::steady_clock::time_point start;		//tiempo inicial
		int currItr;								//'cuantas veces viene imprimiendo determinado tweet
		void displayTweet();						//la funcion que imprime el tweet
		tweetData_t * tweets;						//puntero a los tweets
		unsigned int tweetCount;					//cantidad de tweets
		bool exit;									//bool de salida
		unsigned int delay;							//el delay variable
		unsigned int tweetCursor;					//cursor al tweet (su cambio marca a que tweet moverse)
		BasicLCD * display;							//puntero al LCD
		event_t myEvent;							//evento del dispatcher
};