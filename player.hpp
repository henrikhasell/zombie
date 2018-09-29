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
        //std::cout << "Player contact start." << std::endl;
    }

    virtual void onEndContact(b2Body *other)
    {
        //std::cout << "Player contact end." << std::endl;
    }

    void step()
    {
        weaponCooldown--;
    }
};

struct Zombie : public PhysicsObject
{
    static constexpr float max_hitpoints = 100.0f;
    float hitpoints;

    std::vector<b2Vec2> path;

    Zombie(Game *game, b2Body *body) : PhysicsObject(game, body), hitpoints(max_hitpoints)
    {
        game->calculatePath(body->GetPosition(), game->player->GetPosition(), path);
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
        path.clear();
        game->calculatePath(body->GetPosition(), game->player->GetPosition(), path);

        if(!path.empty())
        {
            constexpr float32 speed = 5.0f;

            const b2Vec2 &head = path[0];
            const b2Vec2 &position = body->GetPosition();

            b2Vec2 direction = head - position;
            float32 distance = direction.Normalize();
            body->SetTransform(position, atan2(direction.x, -direction.y));
            direction *= speed;
            body->SetLinearVelocity(direction);

            if(distance < 0.3f)
            {
                path.erase(path.begin());
            }
        }
    }
};

#endif