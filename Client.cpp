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