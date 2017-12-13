#include "Game.h"
#include <algorithm>


Game::Game(string name) {
    this->name = name;
    prepare_deck();
}

void Game::prepare_deck() {
    pile_of_cards.push_back(new Card());
    pile_of_cards.push_back(new Card());
    pile_of_cards.push_back(new Card());
    std::random_shuffle(pile_of_cards.begin(), pile_of_cards.end());
}

Card * Game::draw_card() {
    Card * result = pile_of_cards.back();
    pile_of_cards.pop_back();
    return result;
}

