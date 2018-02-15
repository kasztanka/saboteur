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
    if (coordinates_are_on_board(x, y))
        return cards[y][x];
    else
        return nullptr;
}

bool Board::coordinates_are_on_board(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

bool Board::card_fits_to_neighbour(TunnelCard *neighbour,
                                   TunnelCard::Direction neighbour_wall_direction,
                                   bool card_wall) {
    return neighbour == nullptr || (neighbour->directions[neighbour_wall_direction] == card_wall);
}

bool Board::card_has_connection_to_root(TunnelCard * new_card, int x, int y) {
    TunnelCard * neighbour;
    list<TunnelCard *> next;
    list<pair<int, int>> coords;
    int neighbour_coords[4][2];
    int neighbour_x, neighbour_y;
    get_neighbours_coordinates(neighbour_coords, x, y);
    bool visited[ROWS][COLS];
    TunnelCard * current;
    bool root_found = false;
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            visited[j][i] = false;
        }
    }

    visited[y][x] = true;
    for (int i = 0; i < 4; i++) {
        neighbour_x = neighbour_coords[i][0];
        neighbour_y = neighbour_coords[i][1];
        if (new_card->directions[i] && (neighbour = get_card(neighbour_x, neighbour_y)) != nullptr) {
            visited[neighbour_y][neighbour_x] = true;
            coords.push_back(make_pair(neighbour_x, neighbour_y));
            next.push_back(neighbour);
        }
    }

    while (!next.empty()) {
        current = next.front();
        next.pop_front();
        x = coords.front().first;
        y = coords.front().second;
        coords.pop_front();
        if (current->is_root) {
            root_found = true;
            break;
        }
        if (current->directions[TunnelCard::MID]) {
            get_neighbours_coordinates(neighbour_coords, x, y);
            for (int i = 0; i < 4; i++) {
                neighbour_x = neighbour_coords[i][0];
                neighbour_y = neighbour_coords[i][1];
                if (current->directions[i]
                        && (neighbour = get_card(neighbour_x, neighbour_y)) != nullptr
                        && !visited[neighbour_y][neighbour_x]) {
                    visited[neighbour_y][neighbour_x] = true;
                    coords.push_back(make_pair(neighbour_x, neighbour_y));
                    next.push_back(neighbour);
                }
            }
        }
    }
    return root_found;
}

void Board::get_neighbours_coordinates(int neighbours_coordinates[4][2], int x, int y) {
    neighbours_coordinates[0][0] = x;
    neighbours_coordinates[0][1] = y - 1;
    neighbours_coordinates[1][0] = x;
    neighbours_coordinates[1][1] = y + 1;
    neighbours_coordinates[2][0] = x - 1;
    neighbours_coordinates[2][1] = y;
    neighbours_coordinates[3][0] = x + 1;
    neighbours_coordinates[3][1] = y;
}

bool Board::validate_tunnel_card(TunnelCard * new_card, int x, int y) {
    return coordinates_are_on_board(x, y)
        && is_spot_for_new_card_free(x, y)
        && card_fits_to_neighbours(new_card, x, y)
        && card_has_connection_to_root(new_card, x, y);
}

void Board::set_card(TunnelCard * card, int x, int y) {
    cards[y][x] = card;
}