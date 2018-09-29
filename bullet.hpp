#ifndef BULLET_HPP
#define BULLET_HPP

#include "object.hpp"
#include <vector>

struct Bullet : public PhysicsObject
{
    Bullet(Game *game, b2Body *body) : PhysicsObject(game, body)
    {
        game->bullets.emplace_back(body);
    }

    virtual ~Bullet()
    {
        std::vector<b2Body*>::iterator iterator = std::find(game->bullets.begin(), game->bullets.end(), body);
    
        if(iterator != game->bullets.end())
        {
            game->bullets.erase(iterator);
        }
    }

    virtual void onBeginContact(b2Body *other)
    {
        PhysicsObject *physicsObject = (PhysicsObject*)other->GetUserData();

        if(physicsObject)
        {
            physicsObject->damage(10.0f);
        }

        remove();
    }

    virtual void onEndContact(b2Body *other)
    {

    }
};
#endif