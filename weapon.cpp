#include "weapon.hpp"
#include "game.hpp"
#include "player.hpp"
#include <iostream>

Weapon weaponList[3] = {
    {
        "Pistol",
        100.0f,
        .use = [](Game *game, b2Body *player)
        {
            std::cout << "Shooting pistol!" << std::endl;

            const float32 rotation = player->GetAngle();

            b2Vec2 startingPosition = player->GetPosition();
            startingPosition.x += sinf(rotation) * 1.5f;
            startingPosition.y -= cosf(rotation) * 1.5f;

            b2Vec2 velocity(
                +sinf(rotation) * 50.0f,
                -cosf(rotation) * 50.0f
            );

            game->createBullet(startingPosition, velocity, rotation);
        }
    },
    {
        "SMG",
        200.0f,
        .use = [](Game *game, b2Body *player)
        {
            std::cout << "Shooting SMG!" << std::endl;
        }
    },
    {
        "Shotgun",
        2000.0f,
        .use = [](Game *game, b2Body *player)
        {
            std::cout << "Shooting Shotgun!" << std::endl;
        }
    },
};