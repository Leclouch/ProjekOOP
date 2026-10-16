#include "Warrior.h"
using namespace std;

// nama, hp, attack, mana, biaya skill
Warrior::Warrior() : Hero("Warrior", 120, 15, 30, 10) {}

string Warrior::getRole() const {return "Warrior";}
string Warrior::getSkillName() const {return "Heavy Strike";}

int Warrior::useSkill(Character& target) {
    if (!target.isAlive() || !spendMana(getSkillCost())) return 0;
    return attack(target, 2.0);
}