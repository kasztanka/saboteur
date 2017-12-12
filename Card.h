#ifndef SABOTEUR_SERVER_CARD_H
#define SABOTEUR_SERVER_CARD_H

#include<string>
using namespace std;


class Card {

};

class TunnelCard: public Card {
public:
    TunnelCard(string name, bool up, bool down, bool left, bool right, bool mid);
    bool up, down, left, right, mid;
    string name;
};


#endif //SABOTEUR_SERVER_CARD_H
