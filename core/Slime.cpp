#include "Slime.h"
using namespace std;

Slime::Slime() : Enemy("Slime", 50, 8) {}

string Slime::takeTurn(Character& hero) {
    int dmg = attack(hero);
    return ("Slime menyerang, " + to_string(dmg) + " damage");
};