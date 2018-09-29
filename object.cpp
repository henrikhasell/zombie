#include "object.hpp"
#include "game.hpp"

PhysicsObject::PhysicsObject(Game *game, b2Body *body) : game(game), body(body), deleted(false)
{

}

void PhysicsObject::remove()
{
    if(!deleted)
    {
        game->delete_list.emplace_back(body);
        deleted = true;
    }
}