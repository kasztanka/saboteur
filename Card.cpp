#include "Card.h"


Card::Card(string name, int type) {
    this->name = name;
    this->type = type;
    is_root = (name.find("start") != string::npos);
}

TunnelCard::TunnelCard(string name, int type) : Card(name, type) {
    directions[UP] = (name.find('U') != string::npos);
    directions[DOWN] = (name.find('D') != string::npos);
    directions[LEFT] = (name.find('L') != string::npos);
    directions[RIGHT] = (name.find('R') != string::npos);
    directions[MID] = (name.find('M') != string::npos);
}

GoalCard::GoalCard(string name, int type) : TunnelCard(name, type) {
    is_gold = (name.find("gold") != string::npos);
}

void TunnelCard::rotate() {
    bool tmp = directions[UP];
    directions[UP] = directions[DOWN];
    directions[DOWN] = tmp;
    tmp = directions[LEFT];
    directions[LEFT] = directions[RIGHT];
    directions[RIGHT] = tmp;
}


BlockCard::BlockCard(string name, int type) : Card(name, type) {
    if (name.find("LAMP") != string::npos) {
        blockade = Card::LAMP;
    } else if (name.find("PICKAXE") != string::npos) {
        blockade = Card::PICKAXE;
    } else {
        blockade = Card::TRUCK;
    }
}