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
            const float32 speed = 5.0f;

            const b2Vec2 &head = path[0];
            const b2Vec2 &position = body->GetPosition();

            b2Vec2 direction = head - position;
            float32 distance = direction.Normalize();

            const float32 rotation = atan2(direction.x, -direction.y);

            if(rotation > 0)
            {
                if(rotation < M_PI / 8)
                {
                    direction = b2Vec2(0.0f, -1.0f);
                }
                else if(rotation < M_PI / 8 + M_PI / 4)
                {
                    direction = b2Vec2(1.0f, -1.0f);
                }
                else if(rotation < M_PI / 8 + M_PI / 4 +  M_PI / 4)
                {
                    direction = b2Vec2(1.0f, 0.0f);
                }
                else if(rotation < M_PI / 8 + M_PI / 4 + M_PI / 4 + M_PI / 4)
                {
                    direction = b2Vec2(1.0f, 1.0f);
                }
                else if(rotation < M_PI / 8 + M_PI / 4 + M_PI / 4 + M_PI / 4 + M_PI / 8)
                {
                    direction = b2Vec2(0.0f, 1.0f);
                }
            }
            else
            {
                if(rotation > -M_PI / 8)
                {
                    direction = b2Vec2(0.0f, -1.0f);
                }
                else if(rotation > -M_PI / 8 - M_PI / 4)
                {
                    direction = b2Vec2(-1.0f, -1.0f);
                }
                else if(rotation > -M_PI / 8 - M_PI / 4 -  M_PI / 4)
                {
                    direction = b2Vec2(-1.0f, 0.0f);
                }
                else if(rotation > -M_PI / 8 - M_PI / 4 - M_PI / 4 - M_PI / 4)
                {
                    direction = b2Vec2(-1.0f, 1.0f);
                }
                else if(rotation > -M_PI / 8 - M_PI / 4 - M_PI / 4 - M_PI / 4 - M_PI / 8)
                {
                    direction = b2Vec2(0.0f, 1.0f);
                }
            }

            direction.Normalize();
            direction *= speed;
            body->SetTransform(position, atan2(direction.x, -direction.y));
            body->SetLinearVelocity(direction);
        }
    }
};

#endif