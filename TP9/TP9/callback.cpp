#include "stdafx.h"
#include "callback.h"
//**************************************  parseCallback  ****************************************************************	

int parseCallback(char *key, char *value, void *UserData)
{
	argument_t *pUserData = (argument_t*) UserData; //Puntero a la estructura
	int result = 0; //Variable para detectar error

	//Evaluo la clave
	if (key != NULL)
	{
		if ((strcasecmp(key, "username")) == 0)
		{
			pUserData->username = value;	//le asigna su valor
			if (*(pUserData->username) == '@')
			{
				pUserData->username++;
			}
			result = 1;
		}
		if ((strcasecmp(key, "tweetcount")) == 0)
		{
			pUserData->tweetcount = atoi(value);	//le asigna su valor
			result = 1;
		}

		
	}
	return result;
}


void how_to_use(void) 
{
	printf("Se deben ingresar los valores para las siguientes opciones: \n");
	printf("-username ... -tweetcount ... ( tweetcount es opcional) \n");
	getchar();
}

int strcasecmp(const char * str1, const char * str2)
{
	char auxStr1[100];
	char auxStr2[100];
	strcpy_s(auxStr1, 100, str1);
	strtolwr(auxStr1);
	strcpy_s(auxStr2, 100, str2);
	strtolwr(auxStr2);

	return strcmp(auxStr1, auxStr2);
}

void strtolwr(char * str)
{
	int i = 0;
	while (*(str + i) != '\0')
	{
		if (isupper(*(str + i)))
		{
			*(str + i) = tolower(*(str + i));
		}
		i++;
	}
}
