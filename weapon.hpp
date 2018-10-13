#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <Box2D/Box2D.h>
#include <string>

struct Game;
struct Player;

struct Weapon
{
    std::string name;
    float delay;

    void (*use)(Game *game, b2Body *player);
};

extern Weapon weaponList[3];

#endif