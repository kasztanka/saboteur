#ifndef SABOTEUR_SERVER_SABOTEURGAME_H
#define SABOTEUR_SERVER_SABOTEURGAME_H

#include <vector>
#include "Card.h"
#include "Board.h"

using namespace std;

class Client;

class Game {
    private:
        vector<Client *> players;
        vector<Card *> pile_of_cards;
        Client * active_player;
        Board * game_board;
        void prepare_deck();
    public:
        Game();
        Card * draw_card();
};


#endif //SABOTEUR_SERVER_SABOTEURGAME_H
