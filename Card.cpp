#include "Card.h"


Card::Card(string name) {
    this->name = name;
}

TunnelCard::TunnelCard(string name) : Card(name) {
    up = (name.find('U') != string::npos);
    down = (name.find('D') != string::npos);
    left = (name.find('L') != string::npos);
    right = (name.find('R') != string::npos);
    mid = (name.find('M') != string::npos);
}