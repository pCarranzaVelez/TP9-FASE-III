#include "stdafx.h"
#include <iostream>
#include <exception>
#include <list>
#include "Dispatcher.h"
#include "Fase1.h"
#include "BasicLCD.h"
#include "Fase2.h"
#include "parselib.h"
#include "Callback.h"
#include "curl/curl.h"
#include "json.hpp"


//Vamos a usar la librería NLOHMANN JSON 
using json = nlohmann::json;




int main(int argc, char ** argv)
{
	argument_t a = { NULL, 15 };
	argument_t *pUserData = &a;
	if (argc == ARGUMENTOS - 1 || argc == ARGUMENTOS + 1)
	{
		if (parseCmdLine(argc, argv, parseCallback,pUserData) == 1)
		{
			json j;                    //Variable donde vamos a guardar lo que devuelva Twitter

			CURL *curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
			CURLM *multiHandle;			//Variable donde vamos a atachear los easy handles
			CURLcode res;
			std::string readString, token;
			BasicLCD * lcd = new Fase2;

			// Query es la dirección de Twitter que vamos a consultar. vamos a bajar los &count twits de screen_name en formato JSON.
			std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";
			std::string accountName = a.username;		
			std::string query2 = "&count=";
			std::string tweetcount = std::to_string(a.tweetcount);
			query += (accountName + query2 + tweetcount);

			// Las dos constantes de abajo son el API_Key y API_SecretKey que necesita Twitter para realizar la autenticación de nuestra App
			// y por lo tanto permitirnos el acceso a sus servidores. Son parte de la estrategia de autenticación Oauth2.
			//Ambas se obtienen de la app que creen en la pagina de Twitter
			std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
			std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";

			tweetData_t * tweets = new tweetData_t[a.tweetcount];

			/************************************************************************************
			*                      Get Bearer Token from the Twitter API						*
			************************************************************************************/

			curl = curl_easy_init();
			if (curl)
			{
				//Seteamos la pagina para buscar el token es siempre la misma
				curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

				// Si nos redirije a algún lado, le decimos que siga dicha redirección.
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

				// Le decimos que trabaje tanto con HTTP como HTTPS, autenticación por HTTP en modo básico.
				curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
				curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

				//Preparamos el password para la autenticación
				std::string userPwd = API_key + ":" + API_SecretKey;
				//Se lo seteamos
				curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

				struct curl_slist *list = NULL;

				//Le decimos a CURL que vamos a mandar URLs codificadas y además en formato UTF8.
				list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

				//Le decimos a CURL que trabaje con credentials.
				std::string data = "grant_type=client_credentials";
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

				//Le decimos a curl que cuando haya que escribir llame a myCallback
				//y que use al string readString como user data.
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

				// Perform the request, res will get the return code
				res = curl_easy_perform(curl);

				// Nos fijamos si hubo algún error
				if (res != CURLE_OK)
				{
					std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
					//Hacemos un clean up de curl antes de salir.
					curl_easy_cleanup(curl);
					delete[] tweets;
					delete lcd;
					return 0;
				}

				// Si no hubo errores hago un clean up antes de realizar un nuevo query.
				curl_easy_cleanup(curl);

				//Si el request fue exitoso, twitter devuelve un JSON
				//Busco el token en el JSON para luego acceder a los twits.
				j = json::parse(readString);


				try
				{
					//Tratamos de acceder al campo acces_token del JSON
					std::string aux = j["access_token"];
					token = aux;
				}
				catch (std::exception& e)
				{
					//Si hubo algun error, se muestra el error que devuelve la libreria
					std::cerr << e.what() << std::endl;
					delete[] tweets;
					delete lcd;
					return 0;
				}
			}
			else
			{
				std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
				delete[] tweets;
				delete lcd;
				return 0;
			}
			//Una vez obtenido el Token ahora voy a buscar los Twits


			/************************************************************************************
			*                        Get Tweets from the Twitter API							*
			************************************************************************************/
			curl = curl_easy_init();
			multiHandle = curl_multi_init();
			readString = "";
			int stillRunning = 0;

			if ((curl != NULL) & (multiHandle != NULL))
			{
				//Attacheo el easy handle para manejar una coneccion no bloqueante.
				curl_multi_add_handle(multiHandle, curl);

				//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
				curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

				//Construimos el Header de autenticacion como lo especifica la API
				//usando el token que obtuvimos antes
				struct curl_slist *list = NULL;
				std::string aux = "Authorization: Bearer ";
				aux = aux + token;
				list = curl_slist_append(list, aux.c_str());
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

				//Seteamos los callback
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

				//Realizamos ahora un perform no bloqueante
				curl_multi_perform(multiHandle, &stillRunning);

				lcd->lcdSetCursorPosition({0,1});
				*lcd << (const unsigned char *)a.username;	//imprime el usuario en la primera linea del display

				while (stillRunning)	//hay que mostrar siempre en la primera linea el nombre de la cuenta que me mandan por cmd
				{
					//Debemos hacer polling de la transferencia hasta que haya terminado
					curl_multi_perform(multiHandle, &stillRunning);

					//	algo que muestre en el display que no se colgó
					lcd->lcdSetCursorPosition({1, 7});
					*lcd << '.';
					Sleep(200);
					*lcd << '.';
					Sleep(200);
					*lcd << '.';
					Sleep(200);
					lcd->lcdSetCursorPosition({1, 6});
					lcd->lcdClearToEOL();
					Sleep(100);
				}
				lcd->lcdSetCursorPosition({ 1,6 });
				lcd->lcdClear();
				//Checkeamos errores
				if (res != CURLE_OK)
				{
					std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
					//Hacemos un clean up de curl antes de salir.
					curl_easy_cleanup(curl);
					delete[] tweets;
					delete lcd;
					return 0;
				}

				//Siempre realizamos el cleanup al final
				curl_easy_cleanup(curl);

				//Si el request fue exitoso, twitter devuelve un JSON
				//con toda la informacion de los tweets que le pedimos

				j = json::parse(readString);	//en esta parte se usaría el dispatcher y los eventos
				try
				{
					//Al ser el JSON un arreglo de objetos JSON se buscan los campos necesarios para cada elemento
					int i = 0;
					for (auto element : j)
					{
						(tweets + i)->text = element["text"].get<std::string>();	
						(tweets + i)->time = stringToDate(element["created_at"].get<std::string>());	//DD/MM/AA - hh:mm
						(tweets + i)->name = element["user"]["name"].get<std::string>();
						i++;
					}
					Dispatcher dispatcher(tweets, a.tweetcount, lcd);
					Sleep(100);
					while (!dispatcher.getExit())
					{

						if(dispatcher.getTimeStatus())
						{
							dispatcher.setEvent({ TIMER_EV, 0 });
							dispatcher.dispatch();
						}
						if (_kbhit())
						{
							char data = _getch();
							dispatcher.setEvent({ KB_EVENT, data });
 							dispatcher.dispatch();
						}
						if(dispatcher.checkLastTweet())
						{
							dispatcher.setEvent({ SOFTWARE_EV, 0 });
							dispatcher.dispatch();
						}
					}

					//std::cout << "Tweets retrieved from Twitter account: " << std::endl;
				}
				catch (std::exception& e)
				{
					//Muestro si hubo un error de la libreria
					std::cerr << e.what() << std::endl;
				}
				

			}
			else
				std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;

			delete[] tweets;
			delete lcd;
			return 0;
		}
		else
		{

			printf("Error de parametros\n");
			how_to_use();
			return 0;
		}
	}
	else
	{

		printf("Error de parametros\n");
		how_to_use();
		return 0;
	}
}
