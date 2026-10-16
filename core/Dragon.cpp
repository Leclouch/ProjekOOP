#include "Dragon.h"
using namespace std;

Dragon::Dragon() : Enemy("Dragon", 150, 14), turnCount(0) {}

string Dragon::takeTurn(Character& hero) {
    if (!isAlive() || !hero.isAlive()) return "";

    turnCount++;
    if (turnCount % 3 == 0) {               // giliran ke 3, 6, 9, dst
        hero.takeDamage(FIRE_DAMAGE);
        return "Dragon menyemburkan api! " + to_string(FIRE_DAMAGE) + " damage";
    }

    int dmg = attack(hero);
    return "Dragon mencakar, " + to_string(dmg) + " damage";
}