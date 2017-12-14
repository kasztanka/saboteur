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
    vector<Game *> * games;

    enum MessageCode {
        INCORRECT_ACTION = -1,
        REQUEST_GAMES = 0,
        CREATE_ROOM = 1,
        JOIN_ROOM,
        ADD_PLAYER,
        CHAT_MESSAGE,
    };

    explicit ClientCommunicator(Client *, vector<Game *> * games);
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
    void create_room();
    void join_room();
    void receive_username();
    void send_players(Game * game);
    void send_new_player_to_others(Game * game);
    void send_games();

};


#endif //SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
