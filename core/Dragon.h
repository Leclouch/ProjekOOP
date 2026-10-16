#pragma once
#include "Enemy.h"

class Dragon : public Enemy {
private:
    int turnCount;                          // ingatan Dragon: sudah berapa giliran
    static constexpr int FIRE_DAMAGE = 30;

public:
    Dragon();
    std::string takeTurn(Character& hero) override;
};