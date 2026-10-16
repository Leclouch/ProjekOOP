#pragma once
#include "Enemy.h"

class Goblin : public Enemy {
public:
    Goblin();
    std::string takeTurn(Character& hero) override;
};