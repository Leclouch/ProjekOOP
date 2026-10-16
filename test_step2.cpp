#include <iostream>
#include "core/Warrior.h"
using namespace std;

int main() {
    Warrior w;
    Character dummy("Dummy", 100, 5);

    cout << w.getName() << " HP " << w.getHp() << "/" << w.getMaxHp()
         << ", Mana " << w.getMana() << "/" << w.getMaxMana() << "\n";

    int dmg = w.useSkill(dummy);
    cout << w.getSkillName() << ": " << dmg << " damage, HP Dummy "
         << dummy.getHp() << ", Mana " << w.getMana() << "\n";

    w.useSkill(dummy);
    w.useSkill(dummy);
    dmg = w.useSkill(dummy);
    cout << "Skill tanpa mana: " << dmg << " damage, Mana " << w.getMana() << "\n";

    w.regenMana(5);
    cout << "Setelah regen: Mana " << w.getMana() << "\n";

    Hero& h = w;    // Warrior dilihat sebagai Hero
    cout << "Lewat Hero&: " << h.getSkillName() << "\n";
}