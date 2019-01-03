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

int main(int argc, char *argv[]) {
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
    int type_user = 3;
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
    citirecomanda:
    do {
        cout << "0.user" << endl;
        cout << "1.admin" << endl;
        cout << "comanda: ";
        cin >> type_user;
    } while (type_user > 1);

    string type_send;
    if (type_user == 0)
        type_send = "0";
    else
        type_send = "1";

    Write(sd, type_send);
    while (1) {
        /* citirea mesajului */


        switch (type_user) {


            case 0 : {
                top:

                cout << "0.Inregistrare" << endl;
                cout << "1.Login" << endl;
                cout << "2.Top" << endl;
                cout << "3.Top-gen" << endl;
                cout << "4.Quit" << endl;

                fflush(stdout);
                do {
                    cout << "Introduceti numarul  comenzii: ";
                    cin >> comanda;
                    cout << comanda << endl;
                    cout << endl;

                } while (comanda > 4 || comanda < 0);


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

                    case 1: {
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
                        read(sd, &size_recive, sizeof(size_recive));
                        bzero(recive, size_recive + 1);
                        read(sd, recive, size_recive);
                        cout << recive << "--" << endl;
                        connected = atoi(recive);
                        switch (connected) {
                            case 0: {
                                cout << "erroare la connetare" << endl;
                                goto top;

                            }
                            case 1: {

                                cout << "Bine ai venit " << name << endl;
                                top_usr_connected:
                                cout << "2.Top" << endl;
                                cout << "3.Top-gen" << endl;
                                cout << "4.Quit" << endl;
                                cout << "5.Vote" << endl;
                                cout << "6.Comment" << endl;
                                cout << "7.Search" << endl;
                                cout << "8.Deconectare" << endl;

                                do {
                                    cout << "Introduceti numarul  comenzii: ";
                                    cin >> comanda;
                                    cout << comanda << endl;
                                    cout << endl;

                                } while (comanda > 8 || comanda < 0);

                                switch (comanda) {
                                    case 2:
                                    {
                                        strcpy(input, "2");
                                        Write(sd, input);
                                    }

                                    case 4: {
                                        strcpy(input, "4");
                                        Write(sd, input);
                                        Close(sd);
                                        return 0;
                                    }

                                    case 5: {
                                        strcpy(input, "5");
                                        Write(sd, input);
                                        char name_song[1024] = "\0";
                                        cout << "Introdu numele melodiei care doresti sa o votezi: ";
                                        cin >> name_song;
                                        Write(sd, name_song);

                                        if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                            perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                            break;

                                        }
                                        bzero(&input, size_recive + 1);

                                        if (read(sd, input, size_recive) <= 0) {
                                            perror("Eroare la read() de la client.\n");
                                            break;

                                        }

                                        if (input == "1")
                                            cout << "Nu s-a votat!" << endl;

                                        else
                                            cout << "S-a votat melodia:" << name_song << endl;

                                        cout << input;
                                        goto top_usr_connected;


                                    }
                                    case 8: {
                                        strcpy(input, "8");

                                        Write(sd, input);
                                        connected = 0;
                                        goto citirecomanda;
                                    }

                                }


                            }

                        }

                    }
                    case 2:{

                        strcpy(input, "2");
                        Write(sd, input);
                    }

                    case 4: {
                        strcpy(input, "4");
                        Write(sd, input);
                        Close(sd);
                        return 0;
                    }
                    case 8: {
                        goto citirecomanda;
                    }

                    default:
                        cout << "comanda gresita,introdua alta" << endl;
                        goto top;
                }


            }
            case 1: {
                top_adm:
                cout << "0.Inregistrare" << endl;
                cout << "1.Login" << endl;
                cout << "2.Top" << endl;
                cout << "3.Top-gen" << endl;
                cout << "4.Quit" << endl;


                fflush(stdout);
                do {
                    cout << "Introduceti numarul  comenzii: ";
                    cin >> comanda;
                    cout << comanda << endl;
                    cout << endl;

                } while (comanda > 8 || comanda < 0);

                switch (comanda) {

                    case 0: {  //inregistrare
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";
                        strcpy(input, "0");
                        Write(sd, input);

                        cout << "Introduceti numele contului dorit: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola dorita: ";
                        cin >> pass;
                        Write(sd, pass);
                        cout << endl << "Introduceti key-ul secret: ";
                        cin >> key;
                        Write(sd, key);

                        goto top_adm;

                    }

                    case 1: {  //login
                        char name[1024] = "\0";
                        char pass[1024] = "\0";
                        char key[1024] = "\0";
                        strcpy(input, "1");
                        Write(sd, input);

                        cout << "Introduceti numele contului: ";
                        cin >> name;
                        Write(sd, name);

                        cout << endl << "Introduceti parola: ";
                        cin >> pass;
                        Write(sd, pass);

                        cout << endl << "Introduceti key: ";
                        cin >> key;
                        Write(sd, key);

                        read(sd, &size_recive, sizeof(size_recive));
                        bzero(recive, size_recive + 1);
                        read(sd, recive, size_recive);
                        cout << recive << "--" << endl;
                        connected = atoi(recive);
                        switch (connected) {
                            case 0: {
                                cout << "erroare la connetare" << endl << endl;
                                goto top_adm;

                            }
                            case 1: {

                                cout << "Bine ai venit " << name << endl;
                                top_adm_connected:
                                cout << "2.Top" << endl;
                                cout << "3.Top-gen" << endl;
                                cout << "4.Quit" << endl;
                                cout << "5.Mute user" << endl;
                                cout << "6.Unmute user" << endl;
                                cout << "7.List users" << endl;
                                cout << "8.Add song" << endl;
                                cout << "9.Deconectare" << endl;

                                fflush(stdout);
                                do {
                                    cout << "Introduceti numarul  comenzii: ";
                                    cin >> comanda;
                                    cout << comanda << endl;
                                    cout << endl;

                                } while (comanda > 9 || comanda < 0);

                                switch (comanda) {

                                    case 2: {  //top
                                        strcpy(input, "2");
                                        Write(sd, input);
                                    }

                                    case 3: {  //top gen
                                        strcpy(input, "3");
                                        Write(sd, input);
                                    }


                                    case 4: { //quit
                                        strcpy(input, "4");
                                        Write(sd, input);
                                        Close(sd);
                                        return 0;
                                    }
                                    case 7: { // lista membrii
                                        int size_memb = 0;
                                        strcpy(input, "7");
                                        Write(sd, input);

                                        read(sd, &size_memb, sizeof(size_memb));
                                        for (int i = 0; i < size_memb; i++) {
                                            if (read(sd, &size_recive, sizeof(size_recive)) <= 0) {
                                                perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                                                break;

                                            }
                                            bzero(&input, size_recive + 1);

                                            if (read(sd, input, size_recive) <= 0) {
                                                perror("Eroare la read() de la client.\n");
                                                break;

                                            }

                                            cout << input << endl;
                                        }

                                        break;
                                    }

                                    case 8: {  //add song
                                        strcpy(input, "8");
                                        Write(sd, input);
                                        char name_song[1024] = "\0";
                                        char link_youtube[1024] = "\0";
                                        char gen[1024] = "\0";
                                        char descriere[1024] = "\0";
                                        char nr_gen[1024] = "\0";

                                        cout << "Numele melodiei: ";
                                        cin >> name_song;
                                        Write(sd, name_song);


                                        cout << endl << "Link youtube: ";
                                        cin >> link_youtube;
                                        Write(sd, link_youtube);

                                        cout << "Descriere melodie: ";
                                        cin >> descriere;
                                        Write(sd, descriere);

                                        cout << endl << "Numar de genuri care apartine melodia: ";
                                        cin >> nr_gen;


                                        Write(sd, nr_gen);

                                        int nr_g = atoi(nr_gen);
                                        for (int i = 0; i < nr_g; i++) {
                                            cout << "Genul melodiei: ";
                                            cin >> gen;
                                            Write(sd, gen);
                                            cout << endl;
                                        }
                                        break;
                                    }
                                    case 9: {  //deconectare
                                        strcpy(input, "9");

                                        Write(sd, input);
                                        connected = 0;
                                        goto citirecomanda;
                                    }

                                    default:
                                        break;
                                }

                                goto top_adm_connected;

                                break;
                            }

                        }

                    }
                    case 2:
                    {
                        strcpy(input, "2");
                        Write(sd, input);
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
}

