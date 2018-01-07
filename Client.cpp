#include "Client.h"


Client::Client(pollfd * client_fd) {
    this->client_fd = client_fd;
    this->active = true;
}

void Client::addCard(Card * new_card) {
    hand_cards.push_back(new_card);
}

Card * Client::getCardByIndex(int card_index) {
    if (hand_cards.size() <= card_index) {
        throw NoCardException();
    }
    return hand_cards.at(card_index);
}

void Client::removeCardByIndex(int card_index) {
    if (hand_cards.size() <= card_index) {
        throw NoCardException();
    }
    hand_cards.erase(hand_cards.begin() + card_index);
}

bool Client::has_blockade(Card::Blockade blockade) {
    return (!blockades.empty()) && find(blockades.begin(), blockades.end(), blockade) != blockades.end();
}

void Client::add_blockade(Card::Blockade blockade) {
    blockades.push_back(blockade);
}

int Client::remove_blockades(vector<Card::Blockade> heals) {
    if (blockades.size() == 0) {
        return 0;
    }
    int healed_blockades = 0;
    for (auto &heal: heals) {
        for (int i = 0; i < blockades.size(); i++) {
            if (blockades.at(i) == heal) {
                blockades.erase(blockades.begin() + i);
                healed_blockades += 1;
                break;
            }
        }
    }
    return healed_blockades;
}

bool Client::is_blocked() {
    return !blockades.empty();
}

void Client::close_connection() {
    cout << "closing connection..." << endl;
    pollfd_mutex.lock();
    if (active) {
        close(client_fd->fd);
        client_fd->fd *= -1;
        active = false;
    }
    pollfd_mutex.unlock();
}