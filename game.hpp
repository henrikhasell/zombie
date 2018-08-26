#ifndef GAME_HPP
#define GAME_HPP

#include <Box2D/Box2D.h>
#include "configuration.hpp"

class Game
{
public:
    b2Body *player;

    Game() : world(b2Vec2(0, 1))
    {
        player = createPlayer(b2Vec2(10.0f, 10.0f));
    }
    void handleInput()
    {
        
    }
    void step()
    {
        world.Step(1.0f/60.0f, 6, 2);
    }
protected:
    void attachBox(b2Body *body, const b2Vec2 &size)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x, size.y);

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.shape = &boxShape;
        fixtureDefinition.density = 1.0f;
        fixtureDefinition.friction = 0.3f;

        body->CreateFixture(&fixtureDefinition);
    }
    b2Body *createPlayer(const b2Vec2 &position)
    {
        b2BodyDef bodyDefinition;
        bodyDefinition.type = b2_dynamicBody;
        bodyDefinition.position = position;
        b2Body *body = world.CreateBody(&bodyDefinition);

        attachBox(body, b2Vec2(1.0f, 1.0f));
        return body;
    }
private:
    b2World world;
};

#endif