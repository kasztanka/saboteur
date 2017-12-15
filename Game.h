#ifndef SABOTEUR_SERVER_GAME_H
#define SABOTEUR_SERVER_GAME_H

#include <vector>
#include <algorithm>
#include <random>
#include "Card.h"
#include "Board.h"

using namespace std;


class Client;


class Game {
private:
    vector<Card *> pile_of_cards;
    Client * active_player;
    Board * game_board;
    void prepare_deck();
    void add_card_to_deck(string, int);
public:
    vector<Client *> players;
    int room_size;
    string name;
    Game(string);
    Card * draw_card();
    void activate_first();
    string get_active_player_username();
    bool is_active_player(Client * client);
    bool has_empty_pile();
};


#endif //SABOTEUR_SERVER_SABOTEURGAME_H
