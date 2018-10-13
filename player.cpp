#include "player.hpp"
#include "weapon.hpp"

Player::Player(Game *game, b2Body *body) :
    PhysicsObject(game, body),
    weapon(&weaponList[0]),
    delay(0)
{

}

void Player::onBeginContact(b2Body *other)
{
    //std::cout << "Player contact start." << std::endl;
}

void Player::onEndContact(b2Body *other)
{
    //std::cout << "Player contact end." << std::endl;
}

void Player::step()
{
    delay -= 1000.0f / 60.0f;
}