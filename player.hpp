#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Box2D/Box2D.h>
#include <iostream>
#include "contact.hpp"
#include "game.hpp"
#include "weapon.hpp"

struct Player : public PhysicsObject
{
    const Weapon *weapon;
    float delay;

    Player(Game *game, b2Body *body);
    virtual void onBeginContact(b2Body *other);
    virtual void onEndContact(b2Body *other);
    void step();
};

struct Zombie : public PhysicsObject
{
    static constexpr float max_hitpoints = 100.0f;
    b2Vec2 direction;
    float hitpoints;

    Zombie(Game *game, b2Body *body) : PhysicsObject(game, body), hitpoints(max_hitpoints)
    {

    }

    virtual ~Zombie()
    {
        std::vector<b2Body*>::iterator iterator = std::find(game->zombies.begin(), game->zombies.end(), body);

        if(iterator != game->zombies.end())
        {
            game->zombies.erase(iterator);
        }
    }

    virtual void onBeginContact(b2Body *other)
    {

    }

    virtual void onEndContact(b2Body *other)
    {

    }

    virtual void damage(float amount)
    {
        hitpoints -= amount;

        if(hitpoints <= 0)
        {
            remove();
        }
    }

    void step()
    {
        b2Vec2 direction;

        if(game->getDirection(body->GetPosition(), direction))
        {
            const float32 speed = 5.0f;
            const float32 rotation = atan2(direction.x, -direction.y);

            body->SetTransform(body->GetPosition(), atan2(direction.x, -direction.y));

            direction.x *= speed;
            direction.y *= speed;

            body->SetLinearVelocity(direction);
        }
    }
};

#endif