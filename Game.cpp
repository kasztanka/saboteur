#include "Game.h"
#include "Client.h"


int Game::ROOM_SIZE;

Game::Game(string name) {
    this->name = name;
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
            for (int i = 0; i < quantity; i++)
                pile_of_cards.push_back(new BlockCard(name, Card::BLOCK));
            break;
        case Card::HEAL:
            for (int i = 0; i < quantity; i++)
                pile_of_cards.push_back(new HealCard(name, Card::HEAL));
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
    add_card_to_deck("LAMP", 3, Card::BLOCK);
    add_card_to_deck("PICKAXE", 3, Card::BLOCK);
    add_card_to_deck("TRUCK", 3, Card::BLOCK);
    add_card_to_deck("LAMP", 2, Card::HEAL);
    add_card_to_deck("PICKAXE", 2, Card::HEAL);
    add_card_to_deck("TRUCK", 2, Card::HEAL);
    add_card_to_deck("LAMP_PICKAXE", 1, Card::HEAL);
    add_card_to_deck("LAMP_TRUCK", 1, Card::HEAL);
    add_card_to_deck("PICKAXE_TRUCK", 1, Card::HEAL);
    random_device rd;
    mt19937 g(rd());
    shuffle(pile_of_cards.begin(), pile_of_cards.end(), g);
}

void Game::prepare_board() {
    gold_card_row = 2;
    coal_card1_row = 0;
    coal_card2_row = 4;
    goal_cards_col = game_board->COLS - 1;
    game_board->set_card(new TunnelCard("UDLRM_start", Card::TUNNEL), 0, 2);
    game_board->set_card(
        new GoalCard("UDLRM_gold", Card::GOAL),
        goal_cards_col,
        gold_card_row
    );
    game_board->set_card(
        new GoalCard("ULM_coal", Card::GOAL),
        goal_cards_col,
        coal_card1_row
    );
    game_board->set_card(
        new GoalCard("URM_coal", Card::GOAL),
        goal_cards_col,
        coal_card2_row
    );
}

void Game::activate_first() {
    active_player_index = 0;
    active_player = players.at(0);
}

void Game::activate_next() {
    active_player_index++;
    active_player_index %= players.size();
    active_player = players.at(active_player_index);
}

Client * Game::get_active_player() {
    return active_player;
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
        activate_next();
    } else {
        throw IncorrectMoveException();
    }
}

string Game::play_block_card(BlockCard * card, int player_index) {
    Client * blocked_player = players[player_index];
    if (!blocked_player->has_blockade(card->blockade)) {
        blocked_player->add_blockade(card->blockade);
        activate_next();
        return blocked_player->username;
    } else {
        throw IncorrectMoveException();
    }
}

string Game::play_heal_card(HealCard * card, int player_index) {
    Client * blocked_player = players[player_index];
    if (blocked_player->remove_blockades(card->blockades) != 0) {
        activate_next();
        return blocked_player->username;
    } else {
        throw IncorrectMoveException();
    }
}

bool Game::is_finished() {
    TunnelCard * gold_card = game_board->get_card(goal_cards_col, gold_card_row);
    return game_board->card_has_connection_to_root(gold_card, goal_cards_col, gold_card_row);
}