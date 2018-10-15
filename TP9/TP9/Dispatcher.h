#pragma once
#include "stdafx.h"
#include "Fase2.h"
#include <string>

using namespace std;
enum EVENT_TYPE { NO_EVENT, TIMER_EV, KB_EVENT, SOFTWARE_EV};

typedef struct
{
	EVENT_TYPE type;
	char data;
}event_t;

typedef struct
{
	std::string name;
	std::string text;
	std::string time;
}tweetData_t;

class Dispatcher
{
	public:
		Dispatcher(tweetData_t * tweets, unsigned int tc, BasicLCD * lcd);
		~Dispatcher();
		void dispatch();
		bool getExit();
		void setEvent(event_t input);
		void setEvent(EVENT_TYPE type, char data);

	private:
		int currItr;
		void displayTweet();
		char * currTweetThread;
		tweetData_t * tweets;
		unsigned int tweetCount;
		bool exit;
		unsigned int delay;
		unsigned int tweetCursor;
		BasicLCD * display;
		event_t myEvent;
};