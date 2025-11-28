/*
 * protocol.h
 *
 * Server header file
 * Definitions, constants and function prototypes for the server
 */
#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Shared application parameters
#define SERVER_PORT 69420  // Server port
#define BUFFER_SIZE 512    // grandezza buffer
#define QUEUE_SIZE 5       // grandezza queue


#define QLEN 6
#define NO_ERROR 0

#define MAX_CITY_LEN 64


void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

//Strutture Dati
typedef struct {
    char tipo;              // 't', 'h', 'w', 'p'
    char citta[MAX_CITY_LEN]; // Nome città
} richiesta_t;

typedef struct {
    unsigned int stato;  // Codice di stato
    char tipo;            // Echo del tipo richiesto
    float val;          // Valore del dato meteo
} risposta_t;


//Prototipi funzioni
int check_city(const char* città);
float get_temp(void);    // -10.0 / 40.0 °C
float get_hum(void);       // 20.0 / 100.0 %
float get_vento(void);           // 0.0 / 100.0 km/h
float get_pres(void);       // 950.0 / 1050.0 hPa


#endif /* PROTOCOL_H_ */
