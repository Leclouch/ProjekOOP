#include "Character.h"
using namespace std;

Character::Character(const string& name, int maxHp, int attackPower)
    : name(name), hp(maxHp), maxHp(maxHp), attackPower(attackPower) {}

const string& Character::getName() const {return name; }
int Character::getHp() const {return hp;}
int Character::getMaxHp() const {return maxHp;}
int Character::getAttackPower() const {return attackPower;}
bool Character::isAlive() const {return hp > 0;}

void Character::takeDamage(int amount) {
    if (amount < 0) return;
    hp -= amount;
    if (hp < 0) hp = 0;
}

void Character::heal(int amount) {
    if (amount < 0 || !isAlive()) return;
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

int Character::attack(Character& target) {
    return attack(target, 1.0);
}

int Character::attack(Character& target, double multiplier) {
    if (!isAlive() || !target.isAlive()) return 0;
    int damage = static_cast<int>(attackPower * multiplier);
    target.takeDamage(damage);
    return damage;
}