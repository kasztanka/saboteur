#include "Board.h"
#include "Card.h"


bool Board::new_card_fits_to_neighbours(TunnelCard * new_card, int x, int y) {
    TunnelCard *up_neigh, *down_neigh, *left_neigh, *right_neigh;
    up_neigh = get_card(x, y - 1);
    down_neigh = get_card(x, y + 1);
    left_neigh = get_card(x - 1, y);
    right_neigh = get_card(x + 1, y);

    if (up_neigh && !new_card->up || down_neigh && !new_card->down ||
        left_neigh && !new_card->left || right_neigh && !new_card->right)
        return false;
    else
        return true;
}

bool Board::is_spot_for_new_card_free(int x, int y) {
    return get_card(x, y) == nullptr;
}

TunnelCard * Board::get_card(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        return cards[y][x];
    else
        return nullptr;
}

bool Board::validate_tunnel_card(TunnelCard * new_card, int x, int y) {
    return is_spot_for_new_card_free(x, y) &&
        new_card_fits_to_neighbours(new_card, x, y);
}