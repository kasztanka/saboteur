#ifndef SABOTEUR_SERVER_CARD_H
#define SABOTEUR_SERVER_CARD_H

#include<string>
using namespace std;


class Card {
public:
    string name;
    explicit Card(string);
};

class TunnelCard: public Card {
public:
    bool up, down, left, right, mid;
    explicit TunnelCard(string);
};


#endif //SABOTEUR_SERVER_CARD_H
