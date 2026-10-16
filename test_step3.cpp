#include <iostream>
#include <vector>
#include "core/Warrior.h"
#include "core/Mage.h"
#include "core/Archer.h"
using namespace std;

int main() {
    vector<Hero*> heroes;
    heroes.push_back(new Warrior());
    heroes.push_back(new Mage());
    heroes.push_back(new Archer());

    // Satu loop, tanpa if atau switch per tipe
    for (Hero* h : heroes) {
        Character dummy("Dummy", 200, 0);
        int dmg = h->useSkill(dummy);
        cout << h->getRole() << " memakai " << h->getSkillName()
             << ": " << dmg << " damage, mana "
             << h->getMana() << "/" << h->getMaxMana() << "\n";
    }

    // Wajib: setiap new punya pasangan delete
    for (Hero* h : heroes) {
        delete h;
    }
}