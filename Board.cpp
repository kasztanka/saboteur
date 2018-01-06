#include "Board.h"


bool Board::card_fits_to_neighbours(TunnelCard * card, int x, int y) {
    TunnelCard *up_neigh, *down_neigh, *left_neigh, *right_neigh;
    up_neigh = get_card(x, y - 1);
    down_neigh = get_card(x, y + 1);
    left_neigh = get_card(x - 1, y);
    right_neigh = get_card(x + 1, y);

    return card_fits_to_neighbour(up_neigh, TunnelCard::DOWN, card->directions[TunnelCard::UP])
        && card_fits_to_neighbour(down_neigh, TunnelCard::UP, card->directions[TunnelCard::DOWN])
        && card_fits_to_neighbour(left_neigh, TunnelCard::RIGHT, card->directions[TunnelCard::LEFT])
        && card_fits_to_neighbour(right_neigh, TunnelCard::LEFT, card->directions[TunnelCard::RIGHT]);
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

bool Board::card_fits_to_neighbour(TunnelCard *neighbour,
                                   TunnelCard::Direction neighbour_wall_direction,
                                   bool card_wall) {
    return neighbour == nullptr || (neighbour->directions[neighbour_wall_direction] == card_wall);
}

bool Board::card_has_connection_to_root(int x, int y) {
    //TODO: implement bfs or dfs search
    return true;
}

bool Board::validate_tunnel_card(TunnelCard * new_card, int x, int y) {
    return is_spot_for_new_card_free(x, y)
        && card_fits_to_neighbours(new_card, x, y)
        && card_has_connection_to_root(x, y);
}

void Board::set_card(TunnelCard * card, int x, int y) {
    cards[y][x] = card;
}