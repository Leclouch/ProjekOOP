#include "Hero.h"
using namespace std;

Hero::Hero(const string& name, int maxHp, int attackPower,int maxMana, int skillCost )
    : Character(name, maxHp, attackPower), mana(maxMana), maxMana(maxMana), skillCost(skillCost) {}

int Hero::getMana() const {return mana;}
int Hero::getMaxMana() const { return maxMana;}
int Hero::getSkillCost() const {return skillCost;}

bool Hero::canUseSkill() const {
    return isAlive() && mana >= skillCost;
}

bool Hero::spendMana(int amount) {
    if (amount < 0 || mana < amount) return false;
    mana -= amount;
    return true;
}

void Hero::regenMana(int amount) {
    if (amount < 0 || !isAlive()) return;
    mana += amount;
    if (mana > maxMana) mana = maxMana;
}

bool Hero::castHeal() {
    if (!isAlive() || !spendMana(HEAL_COST)) return 0;
    heal(HEAL_AMOUNT);
    return 1;
}