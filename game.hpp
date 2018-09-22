#ifndef GAME_HPP
#define GAME_HPP

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include "camera.hpp"
#include "configuration.hpp"
#include "contact.hpp"
#include "marching.hpp"
#include "shape.hpp"
#include "grid.hpp"

class Game
{
public:
    Camera camera;
    ContactListener contactListener;
    std::vector<b2Body*> delete_list;
    std::vector<b2Body*> bullets;
    std::vector<b2Body*> walls;
    b2World world;
    b2Body *player;

    Game();

    void handleInput(const Uint8 *keyboardState);
    void step();
    void buildWalls(const Grid<bool> &tilemap);
    void shootBullet(b2Body *player);
protected:
    void attachBox(b2Body *body, const b2Vec2 &size);
    void attachCircle(b2Body *body, float32 radius);
    b2Body *createPlayerBody(const b2Vec2 &position);
    b2Body *createBullet(
        const b2Vec2 &position,
        const b2Vec2 &velocity,
        float32 angle);
};

#endif