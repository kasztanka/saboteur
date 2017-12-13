#ifndef SABOTEUR_SERVER_BOARD_H
#define SABOTEUR_SERVER_BOARD_H


#include "Card.h"

class Board {
private:
    static const int ROWS = 6;
    static const int COLS = 8;
    TunnelCard * cards[ROWS][COLS];
    TunnelCard * get_card(int x, int y);
    bool new_card_fits_to_neighbours(TunnelCard * new_card, int x, int y);
    bool is_spot_for_new_card_free(int x, int y);
public:
    bool validate_tunnel_card(TunnelCard * new_card, int x, int y);
};


#endif //SABOTEUR_SERVER_BOARD_H
