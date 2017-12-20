#include "Game.h"
#include "Client.h"


Game::Game(string name) {
    this->name = name;
    this->room_size = 2;
    prepare_deck();
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

void Game::play_tunnel_card(TunnelCard * card, int x, int y, bool is_rotated) {
    // TODO: implement int Game and Board
}