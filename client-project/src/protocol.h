/*
 * protocol.h
 *
 * Client header file
 * Definitions, constants and function prototypes for the client
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Shared application parameters
#define SERVER_PORT 69420  // Server port (change if needed)
#define BUFFER_SIZE 512    // Buffer size for messages

#define MAX_CITY_LEN 64

//Prototipi
void StampaGUI(char* IP);

//Strutture Dati
typedef struct {
    char tipo;
    char città[MAX_CITY_LEN];
} richiesta_t;

typedef struct {
    unsigned int stato;
    char tipo;
    float val;
} risposta_t;

#endif /* PROTOCOL_H_ */
