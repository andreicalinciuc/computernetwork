//
// Created by andrei on 22.12.2018.
//

/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include "../Communication/CommunicationHelper.h"
/* codul de eroare returnat de anumite apeluri */
using namespace std;
/* portul de conectare la server*/
int port;

int main (int argc, char *argv[]) {
    int sd;            // descriptorul de socket
    struct sockaddr_in server;    // structura folosita pentru conectare
    // mesajul trimis
    int nr = 0;
    char buf[10];
    char input[1024];
    char recive[1024];
    long   size_recive;
    long size_send;
    /* exista toate argumentele in linia de comanda? */
    if (argc != 3) {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */

    if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    while (1) {
        /* citirea mesajului */

        printf("[client]Introduceti comanda: ");
        fflush(stdout);
        cin.get(input,1024);
        cin.get();

        printf("[client] Am citit %s\n", input);
        /* trimiterea mesajului la server */
        Write(sd,input);




        /* citirea raspunsului dat de server
           (apel blocant pina cind serverul raspunde) */


        if (read(sd, &size_recive, sizeof(size_recive)) < 0) {
            perror("[client]Eroare la read() de la server la dimensiune.\n");
            return errno;
        }
        bzero(&recive, size_recive + 1);

        //momentan doar trimite si primeste, si doar quit, dar primea aiurea inapoi
        if (read(sd, &recive, size_recive) < 0) {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }
        /* afisam mesajul primit */
        printf("[client]Mesajul primit este: %s\n", recive);

        if(strcmp(recive,"quit")==0)
        {
            close(sd);
            break;
        }


    }
    /* inchidem conexiunea, am terminat */

}