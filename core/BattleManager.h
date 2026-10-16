#pragma once
#include <string>
#include <vector>
#include "Hero.h"
#include "Enemy.h"

enum class HeroType { Warrior, Mage, Archer };

class BattleManager {
private:
    Hero* hero;
    std::vector<Enemy*> enemies;
    int currentEnemy;
    std::vector<std::string> log;
    bool gameOver;
    bool playerWon;

    static constexpr int MANA_REGEN = 5;

    // Pembantu internal, tidak untuk UI
    void addLog(const std::string& message);
    void endTurn();

public:
    explicit BattleManager(HeroType type);
    ~BattleManager();

    // Melarang salinan supaya pointer tidak terhapus dua kali
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

    void playerAttack();
    void playerSkill();
    void playerHeal();

    const Hero& getHero() const;
    const Enemy& getCurrentEnemy() const;
    int getEnemyIndex() const;
    int getEnemyCount() const;
    const std::vector<std::string>& getLog() const;
    bool isGameOver() const;
    bool isPlayerWon() const;
};