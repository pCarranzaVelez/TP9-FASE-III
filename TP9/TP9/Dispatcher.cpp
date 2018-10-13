#include "Dispatcher.h"

using namespace std;

Dispatcher::
Dispatcher(string tweets)
{
	this->tweets = tweets;
}

Dispatcher::
~Dispatcher()
{

}

void Dispatcher::
dispatch(EVENT_TYPE event)
{
	switch (event)
	{
	case NO_EVENT:
		break;

	case TIMER_EV:
		break;

	case KB_EVENT:	// R = repite el tweet, S = siguiente, A = anterior, Q = quit, + = mayor velocidad, - = menor velocidad
		break;

	case SOFTWARE_EV:
		break;

	default: break;
	}
}
