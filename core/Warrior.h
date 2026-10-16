#pragma once
#include "Hero.h"

class Warrior : public Hero {
    public:
        Warrior();

        std::string getRole() const override;
        std::string getSkillName() const override;
        int useSkill(Character& target) override;
};