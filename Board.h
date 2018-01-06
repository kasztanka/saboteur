#ifndef SABOTEUR_SERVER_BOARD_H
#define SABOTEUR_SERVER_BOARD_H


#include "Card.h"

class Board {
public:
    static const int ROWS = 5;
    static const int COLS = 10;
    bool validate_tunnel_card(TunnelCard * new_card, int x, int y);
    void set_card(TunnelCard * card, int x, int y);
    bool card_has_connection_to_root(int x, int y);
private:
    TunnelCard * cards[ROWS][COLS];
    bool is_spot_for_new_card_free(int x, int y);
    bool card_fits_to_neighbour(TunnelCard *neighbour, TunnelCard::Direction neighbour_wall, bool card_wall);
    TunnelCard * get_card(int x, int y);
    bool card_fits_to_neighbours(TunnelCard *card, int x, int y);
};


#endif //SABOTEUR_SERVER_BOARD_H
