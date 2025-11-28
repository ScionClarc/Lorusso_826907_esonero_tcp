
/*
 * main.c
 *
 * TCP Server - Template for Computer Networks assignment
 *
 * This file contains the boilerplate code for a TCP server
 * portable across Windows, Linux and macOS.
 */

//Date Modified: 27 nov 2025

#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include <time.h>


int porta;

const char* citta_supportate[] = {"Bari","Roma","Milano","Napoli","Torino","Palermo","Genova","Bologna","Firenze","Venezia"};


int main(int argc, char *argv[]) {
	srand(time(NULL));
	if (argc == 3 && strcmp(argv[1], "-p") == 0){
		porta = atoi(argv[2]);
}
	else
	{
		porta = SERVER_PORT;
}

#if defined WIN32
	//Winsock
	WSADATA wsa;
	int risultato = WSAStartup(MAKEWORD(2,2), &wsa);
	if (risultato != NO_ERROR) {
		printf("Errore in WSAStartup()\n");
		return 0;
	}
#endif

    int iSocket;
	iSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket < 0) {
		printf("Creazione del socket fallita.\n");
		return -1;
	}


	//address
	struct sockaddr_in sock_add;
	sock_add.sin_family = AF_INET;
	sock_add.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sock_add.sin_port = htons(porta);

	if(bind(iSocket, (struct sockaddr *)&sock_add, sizeof(sock_add)) < 0)
	{
		printf("bind() fallita.\n");
		closesocket(iSocket);
		return -1;
	}


	//initialization
	if (listen (iSocket, QLEN) < 0) {
		printf("listen() fallita.\n");
		closesocket(iSocket);
		return -1;
	}



	//server loop
	struct sockaddr_in client_add;
	int client_socket;
	int client_len;
	printf( "In attesa di un client per la connessione...\n");
	while (1) {
	client_len = sizeof(client_add);
	if ((client_socket=accept(iSocket, (struct sockaddr *)&client_add, &client_len)) < 0 )
	{
		printf("accept() failed.\n");
		closesocket(iSocket);
		clearwinsock();
		return -1;
	}




	// Ricezione dati (from client)
	richiesta_t req;
	int n = recv(client_socket, (char*)&req, sizeof(req), 0);
	if (n <= 0) {
		printf("Errore di ricevimento dal client\n");
		closesocket(client_socket);
}
	printf("Richiesta '%c %s' dal client IP %s\n", req.tipo, req.citta, inet_ntoa(client_add.sin_addr));

	// elaborazione
	risposta_t risp;
	memset(&risp, 0, sizeof(risp));

	if (req.tipo != 't' && req.tipo != 'h' && req.tipo != 'w' && req.tipo != 'p'){
		risp.stato = 2;
}
	else if (!is_supported_citta(req.citta)) {
		risp.stato = 1;
}
	else {
		risp.stato = 0;
		risp.tipo = req.tipo;
		switch (req.tipo) {
		case 't':
			{
				risp.val = get_temp();
				break;
}
		case 'h':
			{
				risp.val = get_hum();
				break;
}
		case 'w':
			{
				risp.val = get_vento();
				break;
}
		case 'p':
			{
				risp.val = get_pres();
				break;
}
		}
	}

	//Invio dati
	if (send(client_socket, (char*)&risp, sizeof(risp), 0) != sizeof(risp))
	{
		printf("Errore: messaggio di risposta non recapitato.\n");
	}

	closesocket(client_socket);
	}
	printf("Server terminated.\n");

	closesocket(socket);
	clearwinsock();
	return 0;
}


int is_supported_citta(const char* citta) {
    for (int i = 0; i < 10; i++) {
        const char* c = citta_supportate[i];
        int trovato = 1;

        size_t len_c = strlen(c);
        size_t len_citta = strlen(citta);
        if (len_c != len_citta) continue;

        for (size_t j = 0; j < len_c; j++) {
            if (tolower(c[j]) != tolower(citta[j])) {
                trovato = 0;
                break;
            }
        }
        if (trovato) return 1;
    }
    return 0;
}


float get_temp(void)
{
	return -10.0f + (rand() / (float)RAND_MAX) * 50.0f;
}
float get_hum(void)
{
	return 20.0f + (rand() / (float)RAND_MAX) * 80.0f;
}
float get_vento(void)
{
	return (rand() / (float)RAND_MAX) * 100.0f;
}
float get_pres(void)
{
	return 950.0f + (rand() / (float)RAND_MAX) * 100.0f;
}
