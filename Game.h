#ifndef SABOTEUR_SERVER_GAME_H
#define SABOTEUR_SERVER_GAME_H

#include <vector>
#include <algorithm>
#include <random>
#include "Card.h"
#include "Board.h"

using namespace std;


struct IncorrectMoveException : public exception {};


class Client;


class Game {
private:
    vector<Card *> pile_of_cards;
    Client * active_player;
    int active_player_index;
    Board * game_board;
    int gold_card_row;
    int coal_card1_row;
    int coal_card2_row;
    int goal_cards_col;
    void prepare_deck();
    void prepare_board();
    void add_card_to_deck(string, int, int);
public:
    vector<Client *> players;
    int room_size;
    string name;
    explicit Game(string);
    Card * draw_card();
    void activate_first();
    void activate_next();
    string get_active_player_username();
    bool is_active_player(Client * client);
    bool has_empty_pile();
    void play_tunnel_card(TunnelCard *, int, int, bool);
    string play_block_card(BlockCard *, int);
    bool is_finished();
};


#endif //SABOTEUR_SERVER_SABOTEURGAME_H
