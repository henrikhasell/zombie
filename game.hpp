#ifndef GAME_HPP
#define GAME_HPP

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include "camera.hpp"
#include "configuration.hpp"
#include "contact.hpp"
#include "marching.hpp"
#include "navigation.hpp"
#include "shape.hpp"
#include "grid.hpp"

using namespace Navigation;

class Game
{
public:
    Camera camera;
    Grid <bool> grid;
    ContactListener contactListener;
    PathBuilder pathBuilder;

    std::vector<b2Body*> delete_list;
    std::vector<b2Body*> bullets;
    std::vector<b2Body*> walls;
    std::vector<b2Body*> zombies;

    b2World world;
    b2Body *player;

    Game();

    void handleInput(const Uint8 *keyboardState);
    void step();
    void buildWalls();
    void useWeapon(b2Body *player);
    bool calculatePath(const b2Vec2 &start, const b2Vec2 &finish, std::vector<b2Vec2> &path);

    void attachBox(b2Body *body, const b2Vec2 &size);
    void attachCircle(b2Body *body, float32 radius);
    b2Body *createPlayer(const b2Vec2 &position);
    b2Body *createZombie(const b2Vec2 &position);
    b2Body *createBullet(
        const b2Vec2 &position,
        const b2Vec2 &velocity,
        float32 angle);
};

#endif