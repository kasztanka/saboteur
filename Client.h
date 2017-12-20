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
private:
    vector<Card *> hand_cards;
public:
    pollfd * client_fd;
    mutex pollfd_mutex;
    bool active;
    string username;
    Game * game;
    Client(pollfd * client_fd);
    void addCard(Card *);
    Card * getCardByIndex(int);
    void removeCardByIndex(int);
    void close_connection();
};


#endif //SABOTEUR_SERVER_CLIENT_H
