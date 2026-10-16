#include <iostream>
#include "core/Character.h"
using namespace std;

int main() {
    Character a("Tester", 100, 15);
    Character b("Dummy", 40, 5);

    int dmg = a.attack(b);
    cout << "Serangan biasa: " << dmg << " damage, HP Dummy "
         << b.getHp() << "/" << b.getMaxHp() << "\n";      // 25/40

    dmg = a.attack(b, 2.0);
    cout << "Serangan 2x: " << dmg << " damage, HP Dummy "
         << b.getHp() << "\n";                              // 0

    b.heal(50);
    cout << "Heal saat mati: " << b.getHp() << "\n";       // tetap 0

    a.takeDamage(30);
    a.heal(999);
    cout << "Heal berlebih: " << a.getHp() << "\n";        // 100
}