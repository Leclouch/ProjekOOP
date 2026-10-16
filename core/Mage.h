#pragma once
#include "Hero.h"

class Mage : public Hero {
    public:
        Mage();

        std::string getRole() const override;
        std::string getSkillName() const override;
        int useSkill(Character& target) override;
};