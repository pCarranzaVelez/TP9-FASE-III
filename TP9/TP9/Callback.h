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