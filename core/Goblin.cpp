#include "Goblin.h"
using namespace std;

Goblin::Goblin() : Enemy("Goblin", 70, 6) {}

string Goblin::takeTurn(Character& hero) {
    int first = attack(hero);
    int second = attack(hero);
    return "Goblin menyerang dua kali, " + to_string(first + second) + " damage";
}