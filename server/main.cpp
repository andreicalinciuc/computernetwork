//
// Created by andrei on 22.12.2018.
//
/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include "../Communication/CommunicationHelper.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <mutex>

  std::mutex mu;
/* portul folosit */
#define PORT 2908
using namespace std;
typedef struct thData{
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    int nr;		//mesajul primit de trimis la client
    int sd;		//descriptorul de socket
    int pid;
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i=0;
    char recive[1024];


    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on=1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 5) == -1)
    {
        perror ("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */
    while (1)
    {
        int client;
        thData * td; //parametru functia executata de thread
        int length = sizeof (from);

        printf ("[server]Asteptam la portul %d...\n",PORT);
        fflush (stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ( (client = accept (sd, (struct sockaddr *) &from, (socklen_t *) &length)) < 0)
        {
            perror ("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=i++;
        td->cl=client;

        pthread_create(&th[i], NULL, &treat, td);

    }//while
};

// nu ma folosit threaduri. nu stiu de lace vine asta
static void *treat(void * arg) {



    struct thData tdL;
    tdL = *((struct thData *) arg);

    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());

    raspunde((struct thData *) arg);




    /* am terminat cu acest client, inchidem conexiunea */




};


void raspunde(void *arg)
{

    int  i=0;
    char recive[1024];
    int  size=0;
    struct thData tdL;
    tdL= *((struct thData*)arg);

    while (1) {

        long size_recive = 0;
        long size_send = 0;


        using json = nlohmann::json;
        std:
        ifstream file("../file.json");
        json j;
        file >> j;


        if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            perror("Eroare la read() de la dimesiune\n");

        }
        bzero(&recive, size_recive + 1);

        if (read(tdL.cl, &recive, size_recive) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            perror("Eroare la read() de la client.\n");

        }

        cout <<"Am primit comanda " << recive<<" de la [Thread]:"<<tdL.idThread << endl;
        int comanda = atoi(recive);

        switch (comanda) {
            case 0: {
                char name[1024] = "\0";
                char pass[1024] = "\0";
                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                    break;

                }
                bzero(&name, size_recive + 1);

                if (read(tdL.cl, &name, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");
                    break;

                }


                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                    break;

                }
                bzero(&pass, size_recive + 1);

                if (read(tdL.cl, &pass, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");
                    break;

                }

                mu.lock();
                json x;
                x["name"] = name;
                x["pass"] = pass;
                x["drept de vot"] = 0;
                j["users"].push_back(x);
                ofstream o("../file.json");
                o << setw(4) << j << endl;
                mu.unlock();

                break;
            }
            case 1:
            {
                char name[1024] = "\0";
                char pass[1024] = "\0";
                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune la Inregistrare-name\n");
                    break;

                }
                bzero(&name, size_recive + 1);

                if (read(tdL.cl, &name, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");
                    break;

                }

                if (read(tdL.cl, &size_recive, sizeof(size_recive)) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la dimesiune la Inregistrare-pass\n");
                    break;

                }
                bzero(&pass, size_recive + 1);

                if (read(tdL.cl, &pass, size_recive) <= 0) {
                    printf("[Thread %d]\n", tdL.idThread);
                    perror("Eroare la read() de la client.\n");
                    break;

                }
                        for(auto logi:j["users"])
                        {
                            string usersname=j["users"];
                            cout<<usersname<<endl;
                }
                                break;

            }
            case 4: {
                Close((intptr_t) arg);
                cout << "Am terminat conexiunea cu  [Threadul]: " << tdL.idThread << endl;
                break;

            }
            default:
                break;
        }
        break;
    }


    }
