#pragma once
#include <string>

class Character {
    private:
        std::string name;
        int hp;
        int maxHp;
        int attackPower;
    
    public:
        Character(const std::string& name, int maxHp, int attackPower);
        virtual ~Character() = default;

        //getter
        const std::string& getName() const;
        int getHp() const;
        int getMaxHp() const;
        int getAttackPower() const;
        bool isAlive() const;

        void takeDamage(int amount);
        void heal(int amount);

        //overloading
        int attack(Character& target);
        int attack(Character& target, double multiplier);
};