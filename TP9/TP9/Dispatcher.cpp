#include "Dispatcher.h"

using namespace std;

Dispatcher::
Dispatcher(tweetData_t * tweets, unsigned int tc, BasicLCD * lcd)
{
	this->tweets = tweets;
	this->tweetCount = tc;
	this->tweetCursor = 0;
	this->exit = false;
	this->delay = 10;
	this->display = lcd;
	this->myEvent = { NO_EVENT, 0 };
	this->currItr = 0;
	this->currTweetThread[0] = NULL;
	this->start = chrono::steady_clock::now();
}

Dispatcher::
~Dispatcher()
{

}

bool Dispatcher::
getExit()
{
	return (this->exit);
}

void Dispatcher::
setEvent(event_t input)
{
	setEvent( input.type, input.data );
}
void Dispatcher::
setEvent(EVENT_TYPE type, char data)
{
	if (this->tweetCursor >= this->tweetCount)
	{
		myEvent = { SOFTWARE_EV, 0 };
	}
	
	myEvent = { type, data };
}


void Dispatcher::
dispatch()
{
	switch (myEvent.type)
	{
	case NO_EVENT:
	break;

	case TIMER_EV:
		displayTweet();
	break;

	case KB_EVENT:
	{
		switch (myEvent.data)
		{
			case 'R': case 'r':		//repetir el ultimo tweet
			{
				this->currItr = 0;
				this->currTweetThread[0] = NULL;
			}
			break;
			
			case 'S': case 's':		//Pasa al siguiente tweet
			{
				if (tweetCursor < tweetCount)
				{
					tweetCursor++;
					this->currItr = 0;
					this->currTweetThread[0] = NULL;
				}
			}
			break;
			
			case 'A': case 'a':		//Vuelve al tweet anterior
			{
				if (tweetCursor > 0)
				{
					tweetCursor--;
					this->currItr = 0;
					this->currTweetThread[0] = NULL;
				}
			}
			break;
			
			case 'Q': case 'q':		//Salir
				this->exit = true;
			break;
			
			case '+':				//reducir el delay
				this->delay -= 5;
			break;
			
			case '-':				//aumentar el delay
				this->delay += 5;
			break;
		}
	}
	break;	

	case SOFTWARE_EV:
	{
		*(this->display) << (const unsigned char *) "Ultimo tweet.";
		Sleep(5000);			//cantidad 'prudencial' de tiempo
		this->exit = true;
	}
	break;

	default: break;
	}
}

void Dispatcher::
displayTweet()
{
	this->display->lcdSetCursorPosition({ 0, 0 });
	*(this->display) << (const unsigned char *) ((tweets + tweetCursor)->time.c_str());

	this->display->lcdSetCursorPosition({ 1, 0 });
	string output = (tweets + tweetCursor)->name + ": - " + (tweets + tweetCursor)->text + " -";
	int tweetLength = output.length();
	if(this->currTweetThread[0] == NULL)
	{
		strcpy_s(this->currTweetThread, END_OF_LINE, output.c_str());
		this->currTweetThread[END_OF_LINE] = '\0';
	}
	if (this->currItr < tweetLength)
	{
		strcpy_s(this->currTweetThread, END_OF_LINE, (output.c_str() + currItr));
		this->currTweetThread[END_OF_LINE] = '\0';
		*(this->display) << (unsigned char *) currTweetThread;
		this->currItr++;
	}
	else
	{
		this->currItr = 0;
		this->currTweetThread[0] = NULL;
		this->display->lcdClear();
	}
}

bool Dispatcher::getTimeStatus()
{
	bool ret = false;
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	if (chrono::duration_cast<std::chrono::microseconds>(end - this->start).count() >= delay)
	{
		ret = true;
	}
	this->start = chrono::steady_clock::now();
	return ret;
}