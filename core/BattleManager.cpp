#include "BattleManager.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Slime.h"
#include "Goblin.h"
#include "Dragon.h"
using namespace std;

BattleManager::BattleManager(HeroType type)
    : hero(nullptr), currentEnemy(0), gameOver(false), playerWon(false) {
    switch (type) {
        case HeroType::Warrior: hero = new Warrior(); break;
        case HeroType::Mage:    hero = new Mage();    break;
        case HeroType::Archer:  hero = new Archer();  break;
    }

    enemies.push_back(new Slime());
    enemies.push_back(new Goblin());
    enemies.push_back(new Dragon());

    addLog(hero->getName() + " memasuki arena!");
    addLog(enemies[0]->getName() + " muncul!");
}

BattleManager::~BattleManager() {
    delete hero;
    for (Enemy* e : enemies) {
        delete e;
    }
}

void BattleManager::addLog(const string& message) {
    log.push_back(message);
}

void BattleManager::playerAttack() {
    if (gameOver) return;

    Enemy* enemy = enemies[currentEnemy];
    int dmg = hero->attack(*enemy);
    addLog(hero->getName() + " menyerang " + enemy->getName()
           + ", " + to_string(dmg) + " damage");
    endTurn();
}

void BattleManager::playerSkill() {
    if (gameOver) return;

    if (!hero->canUseSkill()) {
        addLog("Mana tidak cukup untuk " + hero->getSkillName());
        return;                         
    }

    Enemy* enemy = enemies[currentEnemy];
    int dmg = hero->useSkill(*enemy);        // polymorphism
    addLog(hero->getName() + " memakai " + hero->getSkillName()
           + ", " + to_string(dmg) + " damage");
    endTurn();
}

void BattleManager::playerHeal() {
    if (gameOver) return;

    int before = hero->getHp();
    if (!hero->castHeal()) {
        addLog("Mana tidak cukup untuk Heal");
        return;
    }
    addLog(hero->getName() + " memulihkan "
           + to_string(hero->getHp() - before) + " HP");
    endTurn();
}

void BattleManager::endTurn() {
    Enemy* enemy = enemies[currentEnemy];

    if (!enemy->isAlive()) {
        addLog(enemy->getName() + " dikalahkan!");

        if (currentEnemy + 1 < static_cast<int>(enemies.size())) {
            currentEnemy++;
            hero->heal(hero->getMaxHp() * 30 / 100);
            addLog(hero->getName() + " beristirahat dan pulih 30% HP");
            addLog(enemies[currentEnemy]->getName() + " muncul!");
        } else {
            gameOver = true;
            playerWon = true;
            addLog("Semua musuh dikalahkan. Kamu MENANG!");
        }
    } else {
        addLog(enemy->takeTurn(*hero));    

        if (!hero->isAlive()) {
            gameOver = true;
            playerWon = false;
            addLog(hero->getName() + " tumbang. Kamu KALAH.");
            return;
        }
    }

    hero->regenMana(MANA_REGEN);
}

const Hero& BattleManager::getHero() const { return *hero; }
const Enemy& BattleManager::getCurrentEnemy() const { return *enemies[currentEnemy]; }
int BattleManager::getEnemyIndex() const { return currentEnemy; }
int BattleManager::getEnemyCount() const { return static_cast<int>(enemies.size()); }
const vector<string>& BattleManager::getLog() const { return log; }
bool BattleManager::isGameOver() const { return gameOver; }
bool BattleManager::isPlayerWon() const { return playerWon; }