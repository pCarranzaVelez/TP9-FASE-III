#include "Dispatcher.h"

using namespace std;

Dispatcher::
Dispatcher(tweetData_t * tweets, unsigned int tc, BasicLCD * lcd)
{
	this->tweets = tweets;
	this->tweetCount = tc;
	this->tweetCursor = 0;
	this->exit = false;
	this->delay = 1;
	this->display = lcd;
	this->myEvent = { NO_EVENT, 0 };
	this->currItr = 0;
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
				this->display->lcdClear();
			}
			break;
			
			case 'S': case 's':		//Pasa al siguiente tweet
			{
				if (tweetCursor <= tweetCount)
				{
					tweetCursor++;
					this->currItr = 0;
					this->display->lcdClear();
				}
			}
			break;
			
			case 'A': case 'a':		//Vuelve al tweet anterior
			{
				if (tweetCursor > 0)
				{
					tweetCursor--;
					this->currItr = 0;
					this->display->lcdClear();
				}
			}
			break;
			
			case 'Q': case 'q':		//Salir
				this->display->lcdClear();
				this->exit = true;
			break;
			
			case '+':				//reducir el delay
			{
				if (this->delay > 0)
				{
					this->delay -= 1;
				}
			}
				
			break;
			
			case '-':				//aumentar el delay
			{
				this->delay += 1;
			}
				
			break;
		}
	}
	break;	

	case SOFTWARE_EV:
	{
		this->display->lcdClear();
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
	if (currItr == 0)
	{
		display->lcdSetCursorPosition({ 0, 1 });
		*display << (const unsigned char *)((tweets + tweetCursor)->time.c_str());
	}
	display->lcdSetCursorPosition({ 1, 1 });
	string output = (tweets + tweetCursor)->name + ": - " + (tweets + tweetCursor)->text + " -";
	int tweetLength = output.length();
	if (currItr < tweetLength)
	{
		*(display) << (unsigned char *) (output.substr(currItr, 16).c_str());
		currItr++;
	}
	else if (currItr >= (tweetLength - 16))
	{
		*(display) << (unsigned char *)(output.substr(currItr, (tweetLength - 16)).c_str());
		currItr++;
	}
	else
	{
		currItr = 0;
		display->lcdClear();
		this->tweetCursor++;
	}
}

bool Dispatcher::getTimeStatus()
{
	bool ret = false;
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	if (chrono::duration_cast<std::chrono::milliseconds>(end - this->start).count() >= delay)
	{
		ret = true;
	}
	this->start = chrono::steady_clock::now();
	return ret;
}

bool Dispatcher::
checkLastTweet()
{
	bool ret = false;
	if (tweetCursor >= tweetCount -1)
	{
		ret = true;
	}
	return ret;
}