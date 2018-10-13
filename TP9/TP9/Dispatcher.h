#pragma once
#include "stdafx.h"
#include <string>

using namespace std;
enum EVENT_TYPE { NO_EVENT, TIMER_EV, KB_EVENT, SOFTWARE_EV, QUIT};	//¿vamos a tener NO_EVENT?

class Dispatcher
{
	public:
		Dispatcher(string tweets);
		~Dispatcher();
		void dispatch(EVENT_TYPE event);

	private:
		string tweets;
};