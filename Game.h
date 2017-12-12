#ifndef SABOTEUR_SERVER_GAME_H
#define SABOTEUR_SERVER_GAME_H

#include <vector>
using namespace std;

class Client;

class Game {
public:
    vector<Client *> players;
};


#endif //SABOTEUR_SERVER_GAME_H
