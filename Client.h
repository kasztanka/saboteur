#ifndef SABOTEUR_SERVER_CLIENT_H
#define SABOTEUR_SERVER_CLIENT_H

#include <sys/poll.h>
#include <string>
#include <mutex>
#include <unistd.h>
#include <iostream>
#include "Game.h"

using namespace std;


struct NoCardException : public exception {};


class Client {
public:
    pollfd * client_fd;
    mutex pollfd_mutex;
    bool active;
    string username;
    Game * game;
    explicit Client(pollfd *);
    void add_card(Card *);
    Card * get_card_by_index(int);
    void remove_card_by_index(int);
    bool has_blockade(Card::Blockade);
    void add_blockade(Card::Blockade);
    int remove_blockades(vector<Card::Blockade>);
    bool is_blocked();
    void close_connection();
private:
    vector<Card *> hand_cards;
    vector<Card::Blockade> blockades;
};


#endif //SABOTEUR_SERVER_CLIENT_H
