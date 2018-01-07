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


ActionCard::ActionCard(string name, int type) : Card(name, type) {
    if (name.find("LAMP_PICKAXE") != string::npos) {
        blockade = Card::LAMP_PICKAXE;
    } else if (name.find("LAMP_TRUCK") != string::npos) {
        blockade = Card::LAMP_TRUCK;
    } else if (name.find("PICKAXE_TRUCK") != string::npos) {
        blockade = Card::PICKAXE_TRUCK;
    } else if (name.find("LAMP") != string::npos) {
        blockade = Card::LAMP;
    } else if (name.find("PICKAXE") != string::npos) {
        blockade = Card::PICKAXE;
    } else {
        blockade = Card::TRUCK;
    }
}

BlockCard::BlockCard(string name, int type) : ActionCard(name, type) {}

HealCard::HealCard(string name, int type) : ActionCard(name, type) {
    if (name.find("LAMP") != string::npos) {
        blockades.push_back(Card::LAMP);
    }
    if (name.find("PICKAXE") != string::npos) {
        blockades.push_back(Card::PICKAXE);
    }
    if (name.find("TRUCK") != string::npos) {
        blockades.push_back(Card::TRUCK);
    }
}