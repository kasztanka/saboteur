#include "Game.h"
#include "Client.h"


Game::Game(string name) {
    this->name = name;
    this->room_size = 2;
    this->game_board = new Board();
    prepare_deck();
    prepare_board();
}

Card * Game::draw_card() {
    Card * result = pile_of_cards.back();
    pile_of_cards.pop_back();
    return result;
}

void Game::add_card_to_deck(string name, int quantity, int card_type) {
    switch (card_type) {
        case Card::TUNNEL:
            for (int i = 0; i < quantity; i++)
                pile_of_cards.push_back(new TunnelCard(name, Card::TUNNEL));
            break;
        case Card::BLOCK:
            break;
        case Card::HEAL:
            break;
    }
}

void Game::prepare_deck() {
    add_card_to_deck("ULR", 1, Card::TUNNEL);
    add_card_to_deck("LM", 1, Card::TUNNEL);
    add_card_to_deck("DM", 1, Card::TUNNEL);
    add_card_to_deck("LR", 1, Card::TUNNEL);
    add_card_to_deck("UD", 1, Card::TUNNEL);
    add_card_to_deck("UDL", 1, Card::TUNNEL);
    add_card_to_deck("UDLR", 1, Card::TUNNEL);
    add_card_to_deck("DL", 1, Card::TUNNEL);
    add_card_to_deck("DR", 1, Card::TUNNEL);
    add_card_to_deck("UDLRM", 5, Card::TUNNEL);
    add_card_to_deck("LRM", 3, Card::TUNNEL);
    add_card_to_deck("ULM", 4, Card::TUNNEL);
    add_card_to_deck("ULRM", 5, Card::TUNNEL);
    add_card_to_deck("UDRM", 5, Card::TUNNEL);
    add_card_to_deck("UDM", 4, Card::TUNNEL);
    add_card_to_deck("URM", 5, Card::TUNNEL);
    add_card_to_deck("UDM", 2, Card::TUNNEL);
    add_card_to_deck("DLM", 3, Card::TUNNEL);
    random_device rd;
    mt19937 g(rd());
    shuffle(pile_of_cards.begin(), pile_of_cards.end(), g);
}

void Game::prepare_board() {
    //TODO: Add randomness to initializing goal cards
    game_board->set_card(new TunnelCard("UDLRM_start", Card::TUNNEL), 0, 2);
    game_board->set_card(new GoalCard("UDLRM_gold", Card::GOAL), game_board->COLS - 1, 0);
    game_board->set_card(new GoalCard("ULM_coal", Card::GOAL), game_board->COLS - 1, 2);
    game_board->set_card(new GoalCard("URM_coal", Card::GOAL), game_board->COLS - 1, 4);
}

void Game::activate_first() {
    active_player = players.at(0);
}

string Game::get_active_player_username() {
    return active_player->username;
}

bool Game::is_active_player(Client * client) {
    return active_player == client;
}

bool Game::has_empty_pile() {
    return pile_of_cards.empty();
}

void Game::play_tunnel_card(TunnelCard * new_card, int x, int y, bool is_rotated) {
    if (is_rotated) {
        new_card->rotate();
    }
    if (game_board->validate_tunnel_card(new_card, x, y)) {
        game_board->set_card(new_card, x, y);
    } else {
        throw IncorrectMoveException();
    }
}