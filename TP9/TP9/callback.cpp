#include "stdafx.h"
#include "Callback.h"
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

//Concatena lo recibido en content a s
size_t myCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	char* data = (char *)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}

//**************************************************************

std::string stringToDate(std::string date)
{
	std::string day, month, year, hours, minutes, myDate;
	day = date.substr(8, 2);
	month = stringMonthToNumber(date.substr(4, 3));
	year = date.substr(28, 2);
	hours = date.substr(11, 2);
	minutes = date.substr(14, 2);

	myDate = day + "/" + month + "/" + year + " - " + hours + ":" + minutes;
	return myDate;
}

std::string stringMonthToNumber(std::string month)
{
	std::string months[12] = { "Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	std::string month_num;
	for (int i = 0; i < 12; i++)
	{
		if (!strcmp(month.c_str(), months[i].c_str()))
		{
			int realMonth = i + 1;
			if (realMonth < 10)
			{
				month_num = "0" + std::to_string(realMonth);
			}
			month_num = std::to_string(realMonth);
			return month_num;
		}
	}
}