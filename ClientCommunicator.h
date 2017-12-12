#ifndef SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
#define SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H

#include<iostream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include<vector>
#include "Client.h"

class ClientCommunicator {
public:
    Client * client;

    enum Code {
        CHAT_MESSAGE = 1,
    };

    explicit ClientCommunicator(Client *);
    void handle_client_message();
private:
    int receive_int(Client * client);
    string receive_text(Client * client);
    void send_buffer(Client * client, char * buffer, int length);
    void send_int(Client * client, int number);
    void send_text(Client * client, string text, int length);
    void send_int_to_all(vector <Client *> recipients, int number);
    void send_text_to_all(vector <Client *> recipients, string text, int length);
    void handle_chat_message();

};


#endif //SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
