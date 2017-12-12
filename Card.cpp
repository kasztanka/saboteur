#include "Card.h"
#include <string>
using namespace std;

TunnelCard::TunnelCard(string name,
                       bool up,
                       bool down,
                       bool left,
                       bool right,
                       bool mid) {
    this->name = name;
    this->up = up;
    this->down = down;
    this->left = left;
    this->right = right;
    this->mid = mid;
}