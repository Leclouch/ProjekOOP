#pragma once
#include "Enemy.h"

class Slime : public Enemy {
    public:
        Slime();
        std::string takeTurn(Character& hero) override;
};