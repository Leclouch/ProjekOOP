#pragma once
#include <random>
#include "Hero.h"

class Archer : public Hero {
    private:
        std::mt19937 rng;
    public:
        Archer();

        std::string getRole() const override;
        std::string getSkillName() const override;
        int useSkill(Character& target) override;
};