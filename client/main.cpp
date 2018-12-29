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

void quit(char input[1024]);

int main (int argc, char *argv[]) {
    int sd;            // descriptorul de socket
    struct sockaddr_in server;    // structura folosita pentru conectare
    // mesajul trimis
    int nr = 0;
    char buf[10];
    char input[1024];
    char recive[1024];
    long size_recive;
    long size_send;
    int comanda;
    int type_user;
    bool connected;
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
    cout << "0.user" << endl;
    cout << "1.admin" << endl;
    cout << "comanda: ";
    cin >> type_user;
    while (1) {
        /* citirea mesajului */


        switch (type_user)
            case 0 : {
                top:

                cout << "0.Inregistrare" << endl;
                cout << "1.Login" << endl;
                cout << "2.Top" << endl;
                cout << "3.Top-gen" << endl;
                cout << "4.Quit" << endl;

                fflush(stdout);
                do{
                    cout << "Introduceti numarul  comenzii: ";
                    cin >> comanda;
                    cout<<comanda<<endl;
                    cout<<endl;

                }while(comanda>4 || comanda <0);



                switch (comanda) {

                    case 0: {
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        strcpy(input, "0");
                        Write(sd, input);

                        cout << "Introduceti numele contului dorit: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola dorita: ";
                        cin >> pass;
                        Write(sd, pass);

                        goto top;

                    }

                    case 1:{
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        strcpy(input, "1");
                        Write(sd, input);

                        cout << "Introduceti numele contului: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola: ";
                        cin >> pass;
                        Write(sd, pass);

                        goto top;

                    }

                    case 4: {
                        strcpy(input, "4");
                        Write(sd, input);
                        Close(sd);
                        return 0;
                    }

                    default:
                        cout << "comanda gresita,introdua alta" << endl;
                        goto top;
                }


            }

    }
}

