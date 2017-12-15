#ifndef SABOTEUR_SERVER_CARD_H
#define SABOTEUR_SERVER_CARD_H

#include<string>
using namespace std;


class Card {
public:
    string name;
    int type;
    explicit Card(string, int);

    enum CardType {
        TUNNEL = 0,
        BLOCK = 1,
        HEAL = 2
    };
};

class TunnelCard: public Card {
public:
    bool up, down, left, right, mid;
    explicit TunnelCard(string, int);
};


#endif //SABOTEUR_SERVER_CARD_H
