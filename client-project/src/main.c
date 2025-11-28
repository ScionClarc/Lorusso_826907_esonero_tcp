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

risposta_t risp;
richiesta_t req;


int porta;
char *ip = "127.0.0.1";
char tipo = 0;
char città [MAX_CITY_LEN];

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
				printf("Richiesta non valida\n");
				return -1;
			}
			tipo = argv[i + 1][0];
			strncpy(città, argv[i + 1] + 2, MAX_CITY_LEN - 1);
			città[MAX_CITY_LEN - 1] = '\0';
		}
	}


	#if defined WIN32
	// Inizializzazione Winsock
	WSADATA wsa;
	int r = WSAStartup(MAKEWORD(2,2), &wsa);
	if (r != NO_ERROR) {
		printf("Errore in WSAStartup()\n");
		return 0;
	}
	#endif

    //Creazione Socket
    int iSocket;
    iSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (iSocket == -1)
    {
        printf("Creazione del Socket fallita.\n");
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
    if (connect(iSocket, (struct sockaddr*)&indirizzo_socket, sizeof(indirizzo_socket)) < 0)
    {
    	printf("Connessione fallita.\n");
    	closesocket(iSocket);
    	clearwinsock();
    	return -1;
    }

    // Invio di dati al Server
    req.tipo = tipo;
    strncpy(req.città, città, MAX_CITY_LEN - 1);
    req.città[MAX_CITY_LEN - 1] = '\0';

    if (send(iSocket, (char*)&req, sizeof(req), 0) != sizeof(req))
    {
    	printf("Errore: messaggio non recapitato.\n");
    	closesocket(iSocket);
    	clearwinsock();
    	return -1;
    }

    // Ricezione dati dal Server
    int n = recv(iSocket, (char*)&risp, sizeof(risp), 0);
    if (n <= 0) {
    	printf("Errore di ricezione dal server\n");
    	closesocket(iSocket);
    	clearwinsock();
    	return -1;
    }

    MostraGUI(ip);

	printf("Client chiuso.\n");

	closesocket(iSocket);
	clearwinsock();
	return 0;
}


void MostraGUI(char* IP)
{
	printf("Ricevuto risultato dal server ip %s. ", IP);
	if (risp.stato == 0) {
		switch (risp.tipo)
		{
		case 't':
			printf("%s: Temperatura = %.1f°C\n", città, risp.val);
			break;
		case 'h':
			printf("%s: Umidità = %.1f%%\n", città, risp.val);
	        break;
		case 'w':
			printf("%s: Vento = %.1f km/h\n", città, risp.val);
	        break;
		case 'p':
			printf("%s: Pressione = %.1f hPa\n", città, risp.val);
	        break;
		}
	}
	else if (risp.stato == 1)
		{
			printf("Città non disponibile\n");
		}
	else if (risp.stato == 2)
		{
			printf("Richiesta non valida\n");
		}
}
