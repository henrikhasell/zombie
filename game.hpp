#ifndef GAME_HPP
#define GAME_HPP

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include "configuration.hpp"
#include "marching.hpp"
#include "shape.hpp"
#include "grid.hpp"

class Game
{
public:
    std::vector<b2Body*> walls;
    b2Body *player;

    Game() : world(b2Vec2_zero)
    {
        player = createPlayer(b2Vec2(10.0f, 10.0f));
    }

    void handleInput(const Uint8 *keyboardState)
    {
        b2Vec2 velocity = b2Vec2_zero;

        if(keyboardState[SDL_SCANCODE_W])
        {
            velocity.y -= 1.0f;
        }
        if(keyboardState[SDL_SCANCODE_S])
        {
            velocity.y += 1.0f;
        }
        if(keyboardState[SDL_SCANCODE_A])
        {
            velocity.x -= 1.0f;
        }
        if(keyboardState[SDL_SCANCODE_D])
        {
            velocity.x += 1.0f;
        }

        velocity.Normalize();

        if(
            keyboardState[SDL_SCANCODE_W] ||
            keyboardState[SDL_SCANCODE_S] ||
            keyboardState[SDL_SCANCODE_A] ||
            keyboardState[SDL_SCANCODE_D]
        ){
            player->SetTransform(player->GetPosition(), atan2f(velocity.x, -velocity.y));
        }
    
        velocity.x *= PLAYER_SPEED;
        velocity.y *= PLAYER_SPEED;

        player->SetLinearVelocity(velocity);
    }

    void step()
    {
        world.Step(1.0f/60.0f, 6, 2);
    }

    void buildWalls(const Grid<bool> &tilemap)
    {
        for(b2Body *wall : walls)
        {
            world.DestroyBody(wall);
        }
        walls.clear();

        std::vector<Shape> shapes = Marching::solveGrid(tilemap);
        for(const Shape &shape : shapes)
        {
            b2ChainShape chainShape;
            chainShape.CreateChain((b2Vec2*)shape.points.data(), shape.points.size());

            b2FixtureDef fixtureDefinition;
            fixtureDefinition.shape = &chainShape;

            b2BodyDef bodyDefinition;
            b2Body *body = world.CreateBody(&bodyDefinition);
            body->CreateFixture(&fixtureDefinition);
            walls.emplace_back(body);
        }
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
    void attachCircle(b2Body *body, float32 radius)
    {
        b2CircleShape circleShape;
        circleShape.m_radius = radius;

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.shape = &circleShape;
        fixtureDefinition.density = 1.0f;
        fixtureDefinition.friction = 0.3f;

        body->CreateFixture(&fixtureDefinition);
    }
    b2Body *createPlayer(const b2Vec2 &position)
    {
        b2BodyDef bodyDefinition;
        bodyDefinition.type = b2_dynamicBody;
        bodyDefinition.position = position;
        bodyDefinition.fixedRotation = true;
        b2Body *body = world.CreateBody(&bodyDefinition);

        attachCircle(body, 1.0f);
        return body;
    }
private:
    b2World world;
};

#endif