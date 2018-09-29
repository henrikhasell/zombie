#include "game.hpp"
#include "bullet.hpp"
#include "player.hpp"

const bool hardcoded_map[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

Game::Game() : grid(20, 20, hardcoded_map), world(b2Vec2_zero), pathBuilder(grid)
{
    world.SetContactListener(&contactListener);
    player = createPlayer(b2Vec2(10.0f, 10.0f));
    zombies.emplace_back(createZombie(b2Vec2(26.0f, 41.0f)));
    buildWalls();
}


void Game::handleInput(const Uint8 *keyboardState)
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
    if(keyboardState[SDL_SCANCODE_SPACE])
    {
        shootBullet(player);
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

    if(keyboardState[SDL_SCANCODE_UP])
    {
        camera.y -= 1.0f;
    }
    if(keyboardState[SDL_SCANCODE_DOWN])
    {
        camera.y += 1.0f;
    }
    if(keyboardState[SDL_SCANCODE_LEFT])
    {
        camera.x -= 1.0f;
    }
    if(keyboardState[SDL_SCANCODE_RIGHT])
    {
        camera.x += 1.0f;
    }
}

void Game::step()
{
    for(b2Body *body : delete_list)
    {
        PhysicsObject *data = (PhysicsObject*)body->GetUserData();

        if(data)
        {
            body->SetUserData(nullptr);
            delete data;
        }

        world.DestroyBody(body);
    }
    delete_list.clear();

    ((Player*)player->GetUserData())->step();

    for(b2Body *zombie : zombies)
    {
        ((Zombie*)zombie->GetUserData())->step();
    }

    world.Step(1.0f/60.0f, 6, 2);
    const b2Vec2 playerPosition = player->GetPosition();
    camera.lock(playerPosition.x, playerPosition.y);
}

void Game::buildWalls()
{
    for(b2Body *wall : walls)
    {
        world.DestroyBody(wall);
    }
    walls.clear();

    std::vector<Shape> shapes = Marching::solveGrid(grid);
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

void Game::shootBullet(b2Body *player)
{
    Player *playerData = (Player*)player->GetUserData();

    if(playerData->weaponCooldown > 0)
    {
        return;
    }
    else
    {
        playerData->weaponCooldown = 10;
    }

    float32 rotation = player->GetAngle();

    b2Vec2 startingPosition = player->GetPosition();
    startingPosition.x += sinf(rotation) * 1.5f;
    startingPosition.y -= cosf(rotation) * 1.5f;

    b2Vec2 velocity = player->GetLinearVelocity();
    velocity.x += sinf(rotation) * 30.0f;
    velocity.y -= cosf(rotation) * 30.0f;

    createBullet(startingPosition, velocity, rotation);
}

void Game::attachBox(b2Body *body, const b2Vec2 &size)
{
    b2PolygonShape boxShape;
    boxShape.SetAsBox(size.x, size.y);

    b2FixtureDef fixtureDefinition;
    fixtureDefinition.shape = &boxShape;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.3f;

    body->CreateFixture(&fixtureDefinition);
    body->SetBullet(true);
}

void Game::attachCircle(b2Body *body, float32 radius)
{
    b2CircleShape circleShape;
    circleShape.m_radius = radius;

    b2FixtureDef fixtureDefinition;
    fixtureDefinition.shape = &circleShape;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.3f;

    body->CreateFixture(&fixtureDefinition);
}

b2Body *Game::createPlayer(const b2Vec2 &position)
{
    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.position = position;
    bodyDefinition.fixedRotation = true;
    b2Body *body = world.CreateBody(&bodyDefinition);
    body->SetUserData(new Player(this, body));
    attachCircle(body, 1.0f);
    return body;
}

b2Body *Game::createZombie(const b2Vec2 &position)
{
    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.position = position;
    bodyDefinition.fixedRotation = true;
    b2Body *body = world.CreateBody(&bodyDefinition);
    body->SetUserData(new Zombie(this, body));
    attachCircle(body, 1.0f);
    return body;
}

b2Body *Game::createBullet(
        const b2Vec2 &position,
        const b2Vec2 &velocity,
        float32 angle)
{
    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.position = position;
    bodyDefinition.fixedRotation = true;
    bodyDefinition.angle = angle;
    bodyDefinition.linearVelocity = velocity;

    b2Body *body = world.CreateBody(&bodyDefinition);
    body->SetUserData(new Bullet(this, body));
    attachCircle(body, 0.25f);
    return body;
}

bool Game::calculatePath(const b2Vec2 &start, const b2Vec2 &finish, std::vector<b2Vec2> &path)
{
    const int sx = (int)start.x / TILE_W;
    const int sy = (int)start.y / TILE_H;

    const int fx = (int)finish.x / TILE_W;
    const int fy = (int)finish.y / TILE_H;

    std::vector<const Tile *> tile_path;

    bool result = pathBuilder.calculatePath(
        pathBuilder.getTile(sx, sy),
        pathBuilder.getTile(fx, fy),
        tile_path);

    if(result)
    {
        for(const Tile *tile : tile_path)
        {
            path.push_back(b2Vec2(TILE_W * tile->x + (float)TILE_W / 2, TILE_H * tile->y + (float)TILE_H / 2));
        }
    }

    return result;
}