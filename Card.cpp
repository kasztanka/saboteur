#include "Card.h"


Card::Card(string name, int type) {
    this->name = name;
    this->type = type;
}

TunnelCard::TunnelCard(string name, int type) : Card(name, type) {
    up = (name.find('U') != string::npos);
    down = (name.find('D') != string::npos);
    left = (name.find('L') != string::npos);
    right = (name.find('R') != string::npos);
    mid = (name.find('M') != string::npos);
}