#pragma once
#include <string>
#include "Character.h"

class Hero : public Character {
    private:
        int mana;
        int maxMana;
        int skillCost;
    
    protected:
        bool spendMana(int amount);

    public:
        static constexpr int HEAL_AMOUNT = 25;
        static constexpr int HEAL_COST = 10;

        Hero(const std::string& name, int maxHp, int attackPower, int maxMana, int skillCost);

        int getMana() const;
        int getMaxMana() const;
        int getSkillCost() const;
        bool canUseSkill() const;

        void regenMana(int amount);
        bool castHeal();

        virtual std::string getRole() const = 0;
        virtual std::string getSkillName() const = 0;
        virtual int useSkill(Character& target) = 0;
};