#ifndef SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
#define SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
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
        BLOCK,
        HEAL,
        SET_ROLE,
        CLOSE_CONNECTION,
    };

    explicit ClientCommunicator(Client *, vector<Game *> *);
    void handle_client_message();
private:
    int receive_int(Client *);
    string receive_text(Client *);
    void send_buffer(Client *, char *, int);
    void send_int(Client *, int);
    void send_text(Client *, string, int);
    void send_int_to_all(vector <Client *>, int);
    void send_text_to_all(vector <Client *>, string, int);
    void handle_chat_message();
    void send_chat_message(string);
    void create_room();
    void join_room();
    void start_game(Game *);
    int * choose_roles_for_players(int, int);
    void send_card_to_player(Client *, Game *);
    void receive_username();
    void send_players(Game *);
    void send_new_player_to_others(Game *);
    void send_role_to_player(Client *, string);
    void send_games();
    bool username_repeated(Game *, string);
    void send_error_message(Client *, string);
    void send_player_activation(Game *);
    void send_card_to_hand();
    void handle_card_to_board();
    void send_board_card(vector<Client *>, Card *, int, int, bool);
    void send_used_card(int);
    void handle_block_card();
    void send_action_card(ClientCommunicator::MessageCode, vector<Client *>, ActionCard *, string);
    void handle_heal_card();
};


#endif //SABOTEUR_SERVER_CLIENTCOMMUNICATOR_H
