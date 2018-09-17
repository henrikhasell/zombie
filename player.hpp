#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Box2D/Box2D.h>
#include <iostream>
#include "contact.hpp"
#include "game.hpp"

struct Player : public PhysicsObject
{
    int weaponCooldown;

    Player(Game *game, b2Body *body) : PhysicsObject(game, body), weaponCooldown(0)
    {

    }

    virtual void onBeginContact(b2Body *other)
    {
        std::cout << "Player contact start." << std::endl;
    }

    virtual void onEndContact(b2Body *other)
    {
        std::cout << "Player contact end." << std::endl;
    }

    void step()
    {
        weaponCooldown--;
    }
};

#endif