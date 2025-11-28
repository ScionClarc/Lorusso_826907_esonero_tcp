/*
 * main.c
 *
 * TCP Client - Template for Computer Networks assignment
 *
 * This file contains the boilerplate code for a TCP client
 * portable across Windows, Linux and macOS.
 */

//Date Modified: 28 nov 2025

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"

#define NO_ERROR 0

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

weather_response_t risp;
weather_request_t request;


int port;
char *ip = "127.0.0.1";
char type = 0;
char city[MAX_CITY_LEN];

int main(int argc, char *argv[]) {

	int server_port = SERVER_PORT;
	 // Parsing degli argomenti
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
		{
			ip = argv[++i];
		}
		else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
		{
			server_port = atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-r") == 0 && i + 1 < argc)
		{
			if (strlen(argv[i + 1]) < 2)
			{
				printf("Richiesta non valida\n"); return -1;
			}
			type = argv[i + 1][0];
			strncpy(city, argv[i + 1] + 2, MAX_CITY_LEN - 1);
			city[MAX_CITY_LEN - 1] = '\0';
		}
	}


	#if defined WIN32
	// Inizializzazione Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
	#endif

    //Creazione Socket
    int my_socket;
    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (my_socket == -1)
    {
        printf("Creazione della Socket fallita.\n");
        clearwinsock();
        return -1;
    }

    //Assegnazione indirizzo Server
    struct sockaddr_in indirizzo_socket;
    memset(&indirizzo_socket, 0, sizeof(indirizzo_socket));
    indirizzo_socket.sin_family = AF_INET;
    indirizzo_socket.sin_addr.s_addr = inet_addr(ip);
    indirizzo_socket.sin_port = htons(server_port);

    printf("Connessione al server %s sulla porta %d...\n", ip, server_port);

    // Connessione con il Server
    if (connect(my_socket, (struct sockaddr*)&indirizzo_socket, sizeof(indirizzo_socket)) < 0)
    {
    	printf("Connessione fallita.\n");
    	closesocket(my_socket);
    	clearwinsock();
    	return -1;
    }

    // Invio di dati al Server
    request.type = type;
    strncpy(request.city, city, MAX_CITY_LEN - 1);
    request.city[MAX_CITY_LEN - 1] = '\0';

    if (send(my_socket, (char*)&request, sizeof(request), 0) != sizeof(request))
    {
    	printf("Errore: messaggio non recapitato.\n");
    	closesocket(my_socket);
    	clearwinsock();
    	return -1;
    }

    // Ricezione dati dal Server
    int n = recv(my_socket, (char*)&response, sizeof(response), 0);
    if (n <= 0) {
    	printf("Errore di ricezione dal server\n");
    	closesocket(my_socket);
    	clearwinsock();
    	return -1;
    }

    StampaInterfaccia(ip);

	printf("Client terminated.\n");

	closesocket(my_socket);
	clearwinsock();
	return 0;
}


void StampaInterfaccia(char* IP)
{
	printf("Ricevuto risultato dal server ip %s. ", IP);
	if (response.status == 0) {
		switch (response.type)
		{
		case 't':
			printf("%s: Temperatura = %.1f°C\n", city, response.value);
			break;
		case 'h':
			printf("%s: Umidità = %.1f%%\n", city, response.value);
	        break;
		case 'w':
			printf("%s: Vento = %.1f km/h\n", city, response.value);
	        break;
		case 'p':
			printf("%s: Pressione = %.1f hPa\n", city, response.value);
	        break;
		}
	}
	else if (response.status == 1)
		{
			printf("Città non disponibile\n");
		}
	else if (response.status == 2)
		{
			printf("Richiesta non valida\n");
		}
}