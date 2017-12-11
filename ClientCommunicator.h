#ifndef SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
#define SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H

#include<iostream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include "Client.h"

class ClientCommunicator {
public:
    Client * client;
    int client_descriptor;

    enum Code {
        MESSAGE = 1,
    };

    explicit ClientCommunicator(Client *);
    void handle_client_communicate();
private:
    int receive_int(int descriptor);
    string receive_text(int descriptor);
    void send_buffer(int descriptor, char * buffer, int length);
    void send_int(int socket, int number);
    void send_text(int socket, string text, int length);
    void close_connection(Client * client);
    void handle_message();

};


#endif //SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
