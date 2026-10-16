#include "Enemy.h"
using namespace std;

Enemy::Enemy(const string& name, int maxHp, int attackPower)
        : Character(name, maxHp, attackPower) {}