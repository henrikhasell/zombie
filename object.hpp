#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Box2D/Box2D.h>
#include <iostream>

class Game;

class PhysicsObject
{
public:
    PhysicsObject(Game *game, b2Body *body);
    virtual ~PhysicsObject() = default;
    virtual void onBeginContact(b2Body *other) = 0;
    virtual void onEndContact(b2Body *other) = 0;
    virtual void damage(float amount) { std::cout << "Damaging " << this << " by " << amount << std::endl; }
    void remove();
protected:
    Game *game;
    b2Body *body;
private:
    bool deleted;
};

#endif