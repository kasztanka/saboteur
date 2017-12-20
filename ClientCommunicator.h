#ifndef SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
#define SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H

#include<iostream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include<vector>
#include "Client.h"


struct ConnectionBrokenException : public exception {};
struct ConnectionClosedException : public exception {};


class ClientCommunicator {
public:
    Client * client;
    vector<Game *> * games;

    enum MessageCode {
        INCORRECT_ACTION = -1,
        REQUEST_GAMES,
        CREATE_ROOM,
        JOIN_ROOM,
        ADD_PLAYER,
        CHAT_MESSAGE,
        START_GAME,
        ACTIVATE_PLAYER,
        DRAW_CARD,
        ADD_CARD_TO_BOARD,
        REMOVE_CARD_FROM_HAND,
        CLOSE_CONNECTION,
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
    void start_game(Game *);
    Card * send_card_to_player(Client *, Game *);
    void receive_username();
    void send_players(Game * game);
    void send_new_player_to_others(Game * game);
    void send_games();
    bool username_repeated(Game * game, string username);
    void send_error_message(Client * client, string error_message);
    void send_player_activation(Game * game);
    void send_card_to_hand();
    void handle_card_to_board();
    void send_board_card(vector<Client *>, Card *, int, int, bool);
    void send_used_card(int);
};


#endif //SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
