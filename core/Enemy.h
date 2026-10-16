#pragma once
#include <string>
#include "Character.h"

class Enemy : public Character {
    public:
        Enemy(const std::string& name, int maxHp, int attackPower);
        virtual std::string takeTurn(Character& hero) = 0;
};

