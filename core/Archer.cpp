#include "Archer.h"
using namespace std;

// nama, hp, attack, mana, biaya skill
Archer::Archer() : Hero("Archer", 100, 12, 40, 15),
                    rng(random_device{}()) {}

string Archer::getRole() const {return "Archer";}
string Archer::getSkillName() const {return "Double Shot";}

int Archer::useSkill(Character& target) {
    if (!target.isAlive() || !spendMana(getSkillCost())) return 0;
    int total = attack(target, 1.5);
    uniform_int_distribution<int> coin(0,1);
    if (coin(rng) == 1){
        total += attack(target, 1.5);
    }
    return total;
}