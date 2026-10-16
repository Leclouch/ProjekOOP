#include "Mage.h"
using namespace std;

// nama, hp, attack, mana, biaya skill
Mage::Mage() : Hero("Mage", 80, 10, 60, 10) {}

string Mage::getRole() const {return "Mage";}
string Mage::getSkillName() const {return "Fireball";}

int Mage::useSkill(Character& target) {
    if (!target.isAlive() || !spendMana(getSkillCost())) return 0;
    return attack(target, 3.0);
}