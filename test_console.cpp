#include <iostream>
#include "core/BattleManager.h"
using namespace std;

void printStatus(const BattleManager& battle) {
    const Hero& h = battle.getHero();
    const Enemy& e = battle.getCurrentEnemy();
    cout << "\n" << h.getName() << "  HP " << h.getHp() << "/" << h.getMaxHp()
         << "  Mana " << h.getMana() << "/" << h.getMaxMana() << "\n";
    cout << e.getName() << " (" << battle.getEnemyIndex() + 1 << "/"
         << battle.getEnemyCount() << ")  HP " << e.getHp() << "/" << e.getMaxHp() << "\n";
}

int main() {
    cout << "Pilih hero: 1 Warrior, 2 Mage, 3 Archer\n> ";
    int pick;
    cin >> pick;

    HeroType type = HeroType::Warrior;
    if (pick == 2) type = HeroType::Mage;
    else if (pick == 3) type = HeroType::Archer;

    BattleManager battle(type);
    size_t shown = 0;    // jumlah baris log yang sudah dicetak

    while (true) {
        const vector<string>& log = battle.getLog();
        for (; shown < log.size(); shown++) {
            cout << "  " << log[shown] << "\n";
        }
        if (battle.isGameOver()) break;

        printStatus(battle);
        const Hero& h = battle.getHero();
        cout << "1 Attack   2 " << h.getSkillName() << " (" << h.getSkillCost()
             << " mana)   3 Heal (" << Hero::HEAL_COST << " mana)\n> ";

        int action;
        if (!(cin >> action)) break;
        if (action == 1) battle.playerAttack();
        else if (action == 2) battle.playerSkill();
        else if (action == 3) battle.playerHeal();
    }

    cout << (battle.isPlayerWon() ? "\n=== MENANG ===\n" : "\n=== KALAH ===\n");
}