#include <iostream>
#include <vector>
#include "core/Slime.h"
#include "core/Goblin.h"
#include "core/Dragon.h"
using namespace std;

int main() {
    vector<Enemy*> enemies;
    enemies.push_back(new Slime());
    enemies.push_back(new Goblin());
    enemies.push_back(new Dragon());

    Character dummy("Dummy", 500, 0);

    for (int round = 1; round <= 3; round++) {
        cout << "Ronde " << round << "\n";
        for (Enemy* e : enemies) {
            cout << "  " << e->takeTurn(dummy) << "\n";
        }
    }
    cout << "HP Dummy: " << dummy.getHp() << "\n";

    for (Enemy* e : enemies) {
        delete e;
    }
}