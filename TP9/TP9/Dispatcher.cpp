#include "Dispatcher.h"

using namespace std;

Dispatcher::
Dispatcher(tweetData_t * tweets, unsigned int tc, BasicLCD * lcd)
{
	this->tweets = tweets;
	this->tweetCount = tc;
	this->tweetCursor = 0;
	this->exit = false;
	this->delay = 500;
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

//setters del evento
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

/*
* dispatch() es el dispatcher en si. Se llama cada vez que hay una situacion que cambie el campo metodo.
*/
void Dispatcher::
dispatch()
{
	switch (myEvent.type)
	{
	case NO_EVENT:
	break;

	case TIMER_EV:				//cada vez que pase un tick del timer, se hace un update del display
		displayTweet();
	break;

	case KB_EVENT:				//si se apreta una tecla, hay distintas opciones:
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
			{
				this->display->lcdClear();
				this->exit = true;
			}
			break;
			
			case '+':				//reducir el delay
			{
				if (this->delay > 400)
				{
					this->delay -= 250;
				}
			}
				
			break;
			
			case '-':				//aumentar el delay
			{
				if (delay < 2000)
				{
					this->delay += 250;
				}
			}
				
			break;
			default: break;
		}
	}
	break;	

	case SOFTWARE_EV:		//el software event seria cuando ya se mostraron todos los tweets
	{
		this->display->lcdClear();
		*(this->display) << (const unsigned char *) "Ultimo tweet.";
		Sleep(5000);			//cantidad 'prudencial' de tiempo
		this->display->lcdClear();
		this->exit = true;
	}
	break;

	default: break;
	}
}

/*
* displayTweet() pone el tweet en el lcd.
*/
void Dispatcher::
displayTweet()
{
	if (currItr == 0) //si es la primera vez que se empieza a imprimir el tweet, se muestra el tiempo en la primera linea
	{
		display->lcdSetCursorPosition({ 0, 1 });
		*display << (const unsigned char *)((tweets + tweetCursor)->time.c_str());
	}
	display->lcdSetCursorPosition({ 1, 1 });
	string output = (tweets + tweetCursor)->name + ": - " + (tweets + tweetCursor)->text + " -";
	int tweetLength = output.length();
	if (currItr < tweetLength)
	{
		if (currItr >= (tweetLength - END_OF_LINE))
		{
			*(display) << (unsigned char *)(output.substr(currItr, (tweetLength - END_OF_LINE)).c_str());
			display->lcdSetCursorPosition({ 1, ((tweetLength - END_OF_LINE) + currItr -1) });
			display->lcdClearToEOL();
			currItr++;
		}
		else
		{
			*(display) << (unsigned char *)(output.substr(currItr, 16).c_str());
			currItr++;
		}
	}
	else
	{
		currItr = 0;
		display->lcdClear();
		this->tweetCursor++;
	}
}

/*
* El timer compara un tiempo 'start'(inicialmente determinado al construir la clase, 
* reescrito cada vez que se cumple un 'tick') y un tiempo 'end', defeinido cada vez que se llama la funcion.
* Devuelve true solo si la diferencia entre end y start es mayor al campo delay del dispatcher.
*/
bool Dispatcher::getTimeStatus()
{
	bool ret = false;
	
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	if (chrono::duration_cast<std::chrono::milliseconds>(end - this->start).count() >= delay)
	{
		ret = true;
		this->start = chrono::steady_clock::now();
	}
	
	return ret;
}

bool Dispatcher::
checkLastTweet()
{
	bool ret = false;
	if (tweetCursor >= tweetCount)
	{
		ret = true;
	}
	return ret;
}