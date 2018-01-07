#ifndef SABOTEUR_SERVER_CARD_H
#define SABOTEUR_SERVER_CARD_H

#include<string>
#include <vector>
using namespace std;


class Card {
public:
    string name;
    int type;
    explicit Card(string, int);

    enum CardType {
        TUNNEL = 0,
        BLOCK = 1,
        HEAL = 2,
        GOAL = 3
    };

    enum Blockade {
        LAMP = 1,
        PICKAXE,
        TRUCK,
        LAMP_PICKAXE,
        LAMP_TRUCK,
        PICKAXE_TRUCK,
    };
};

class TunnelCard: public Card {
public:
    enum Direction {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        MID,
    };
    bool directions[5];
    bool is_root;
    explicit TunnelCard(string, int);
    void rotate();
};

class GoalCard: public TunnelCard {
public:
    bool is_gold;
    explicit GoalCard(string, int);
};

class ActionCard: public Card {
public:
    Card::Blockade blockade;
    explicit ActionCard(string, int);
};

class BlockCard: public ActionCard {
public:
    explicit BlockCard(string, int);
};

class HealCard: public ActionCard {
public:
    vector<Card::Blockade> blockades;
    explicit HealCard(string, int);
};


#endif //SABOTEUR_SERVER_CARD_H
