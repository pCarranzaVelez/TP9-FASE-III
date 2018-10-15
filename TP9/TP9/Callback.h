#pragma once
#include "stdafx.h"
#include "parselib.h"

#define ARGUMENTOS 4

typedef struct
{
	char * username;
	int tweetcount;
}argument_t;


int parseCallback(char *key, char *value, void *UserData);
void how_to_use(void);

int strcasecmp(const char * str1, const char * str2);
void strtolwr(char * str);

//Funciones auxiliares
size_t myCallback(void *contents, size_t size, size_t nmemb, void *userp);

//quizas moverlas despues

std::string stringMonthToNumber(std::string month);
std::string stringToDate(std::string date);