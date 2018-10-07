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
    // zombies.emplace_back(createZombie(b2Vec2(26.0f, 45.0f)));
    // zombies.emplace_back(createZombie(b2Vec2(26.0f, 49.0f)));
    // zombies.emplace_back(createZombie(b2Vec2(26.0f, 53.0f)));
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
        useWeapon(player);
    }

    float32 magnitude = velocity.Normalize();

    if(magnitude > 0.0f)
    {
        player->SetTransform(player->GetPosition(), atan2f(velocity.x, -velocity.y));

        velocity.x *= PLAYER_FORCE;
        velocity.y *= PLAYER_FORCE;
    }

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
    const b2Vec2 playerPosition = player->GetPosition();

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

    pathBuilder.calculatePath(
        pathBuilder.getTile(
            (int)(playerPosition.x / TILE_W),
            (int)(playerPosition.y / TILE_H)
        )
    );

    ((Player*)player->GetUserData())->step();

    for(b2Body *zombie : zombies)
    {
        ((Zombie*)zombie->GetUserData())->step();
    }

    world.Step(1.0f/60.0f, 6, 2);
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

void Game::useWeapon(b2Body *player)
{
    Player *player_data = (Player*)player->GetUserData();

    if(player_data->delay > 0)
    {
        return;
    }

    player_data->delay = player_data->weapon->delay;
    player_data->weapon->use(this, player);
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

bool Game::calculatePath(const b2Vec2 &location, b2Vec2 &direction)
{
    const int x = (int)location.x / TILE_W;
    const int y = (int)location.y / TILE_H;

    std::vector<const Tile *> tile_path;

    const Tile *tile = pathBuilder.getTile(x, y);

    if(tile && tile->parent)
    {
        direction.x = (float32)(tile->parent->x - tile->x);
        direction.y = (float32)(tile->parent->y - tile->y);
        direction.Normalize();
    }

    return tile && tile->parent;
}