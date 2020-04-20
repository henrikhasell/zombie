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
    zombies.emplace_back(createZombie(b2Vec2(8.5 * TILE_W, 10.5 * TILE_H)));
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

    pathBuilder.loadGrid(grid);
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

bool Game::quickRayCast(const b2Vec2 &start, const b2Vec2 &finish)
{
    b2Vec2 ray_direction = finish - start;
    ray_direction.Normalize();

    int stepX = ray_direction.x >= 0 ? 1 : -1;
    int stepY = ray_direction.y >= 0 ? 1 : -1;

    float distanceFromEdgeX = stepX < 0 ? -fmod(start.x, TILE_W) : TILE_W - fmod(start.x, TILE_W);

    float distanceFromEdgeY = stepY < 0 ? -fmod(start.y, TILE_H) : TILE_H - fmod(start.y, TILE_H);

    float magnitudeToEdgeX = distanceFromEdgeX / ray_direction.x;
    float magnitudeToEdgeY = distanceFromEdgeY / ray_direction.y;

    float magnitudeForOneUnitX = TILE_W / fabs(ray_direction.x);
    float magnitudeForOneUnitY = TILE_H / fabs(ray_direction.y);

    Tile *current_tile = pathBuilder.getTileSafely(start.x / TILE_W, start.y / TILE_H);
    Tile *target_tile = pathBuilder.getTileSafely(finish.x / TILE_W, finish.y / TILE_H);

    if(!current_tile || !target_tile)
    {
        return false;
    }

    while(current_tile)
    {
        int X = current_tile->x;
        int Y = current_tile->y;

        if(!current_tile->navigable)
        {
            return false;
        }

        if(magnitudeToEdgeX < magnitudeToEdgeY)
        {
            magnitudeToEdgeX += magnitudeForOneUnitX;
            X += stepX;
        }
        else
        {
            magnitudeToEdgeY += magnitudeForOneUnitY;
            Y += stepY;
        }

        if(current_tile == target_tile)
        {
            break;
        }

        current_tile = pathBuilder.getTileSafely(X, Y);
    }

    return true;
}


bool Game::visible(const b2Vec2 &start, const b2Vec2 &finish)
{
    b2Vec2 diff = start - finish;
    diff.Normalize();

    const float angle = atan2f(diff.x, -diff.y);
    constexpr float width = 1.;

    b2Vec2 a_s = b2Vec2(
            start.x + cosf(angle) * width,
            start.y + sinf(angle) * width
    );

    b2Vec2 b_s = b2Vec2(
            start.x - cosf(angle) * width,
            start.y - sinf(angle) * width
    );

    b2Vec2 a_f = b2Vec2(
            finish.x + cosf(angle) * width,
            finish.y + sinf(angle) * width
    );

    b2Vec2 b_f = b2Vec2(
            finish.x - cosf(angle) * width,
            finish.y - sinf(angle) * width
    );

    return quickRayCast(start, finish) && quickRayCast(a_s, a_f) && quickRayCast(b_s, b_f);
}

std::vector<b2Vec2> Game::calculatePath(const b2Vec2 &location)
{
    const int x = (int)location.x / TILE_W;
    const int y = (int)location.y / TILE_H;

    std::vector<b2Vec2> tile_path;

    tile_path.push_back(location);

    const Tile *tile = pathBuilder.getTile(x, y);

    while(tile)
    {
        tile_path.push_back(getTilePosition(tile));
        tile = tile->parent;
    }

    std::vector<b2Vec2> minimal_path;
    b2Vec2 previousViewpoint = tile_path[0], currentViewpoint = tile_path[0];

    for(const b2Vec2 &node : tile_path)
    {
        if(minimal_path.empty() || !visible(currentViewpoint, node))
        {
            minimal_path.push_back(previousViewpoint);
            currentViewpoint = previousViewpoint;
        }
        previousViewpoint = node;
    }
    minimal_path.push_back(player->GetPosition());
    return minimal_path;
}

const Tile *Game::getNextTile(const b2Vec2 &location)
{
    const int x = (int)location.x / TILE_W;
    const int y = (int)location.y / TILE_H;
    const Tile *tile = pathBuilder.getTileSafely(x, y);
    return tile ? tile->parent : nullptr;
}

bool Game::getDirection(const b2Vec2 &location, b2Vec2 &direction)
{
    auto path = calculatePath(location);

    if(path.size() < 2) {
        return false;
    }

    direction = path[1] - path[0];
    direction.Normalize();

    return true;
}

b2Vec2 getTilePosition(const Tile *tile)
{
    return b2Vec2(
            ((float32)tile->x + 0.5f) * TILE_W,
            ((float32)tile->y + 0.5f) * TILE_H
    );
}