#include <cfloat>
#include <cmath>
#include <list>

#include "navigation.hpp"

using namespace Navigation;

float Tile::heuristic(const Tile *start, const Tile *destination) const
{
    const float relative_x = (float)(this->x - destination->x);
    const float relative_y = (float)(this->y - destination->y);

    return cost + sqrtf(relative_x * relative_x + relative_y * relative_y);
}

void Tile::initialise(int x, int y, bool navigable)
{
    this->x = x;
    this->y = y;
    this->navigable = navigable;

    reset();
}

bool Tile::navigate(const Tile *parent, float cost)
{
    if(navigable)
    {
        float total_cost = parent->cost + cost;

        if(total_cost < this->cost)
        {
            this->cost = total_cost;
            this->parent = parent;

            return true;
        }
    }

    return false;
}

void Tile::reset()
{
    parent = nullptr;
    cost = FLT_MAX;
}

PathBuilder::PathBuilder(const Grid<bool> &grid) : width(grid.w), height(grid.h)
{
    tiles = new Tile[width * height];

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            getTile(x, y)->initialise(x, y, !grid.getTile(x, y));
        }
    }
}

PathBuilder::~PathBuilder()
{
    delete[] tiles;
}

Tile *PathBuilder::getTile(int x, int y)
{
    return tiles + x + width * y;
}

Tile *PathBuilder::getTileSafely(int x, int y)
{
    return (x > 0 && y > 0 && x < width && y < height) ? getTile(x, y) : nullptr;
}

static void priority_insert(
    std::list<Tile*> &list,
    Tile *tile,
    Tile *start,
    Tile *destination)
{
    auto i = list.begin();

    while(
        i != list.end() &&
        (*i)->heuristic(start, destination) <
        tile->heuristic(start, destination))
    {
        i++;
    }

    list.insert(i, tile);
}

bool PathBuilder::calculatePath(Tile *start, Tile *finish)
{
    reset();

    std::list<Tile*> open_set;
    open_set.push_back(finish);

    finish->cost = 0.0f;

    while(!open_set.empty())
    {
        const Tile *head = open_set.front();
        const int x = head->x;
        const int y = head->y;

        open_set.pop_front();


        Tile *adjacent[4] = {
                getTileSafely(x + 1, y + 0),
                getTileSafely(x - 1, y + 0),
                getTileSafely(x + 0, y + 1),
                getTileSafely(x + 0, y - 1)
        };

        for(Tile *selected : adjacent)
        {
            if(selected && selected->navigate(head, 1.0f))
            {
                if(selected == start)
                {
                    return true;
                }
                else
                {
                    priority_insert(open_set, selected, finish, start);
                }
            }
        }

        std::vector<Tile*> diagonal;
        Tile *tmp1, *tmp2;

        tmp1 = getTileSafely(x - 1, y + 0);
        tmp2 = getTileSafely(x + 0, y + 1);

        if(tmp1 && tmp1->navigable && tmp2 && tmp2->navigable)
        {
            diagonal.push_back(getTile(x - 1, y + 1));
        }

        tmp1 = getTileSafely(x + 1, y + 0);
        tmp2 = getTileSafely(x + 0, y + 1);

        if(tmp1 && tmp1->navigable && tmp2 && tmp2->navigable)
        {
            diagonal.push_back(getTile(x + 1, y + 1));
        }

        tmp1 = getTileSafely(x - 1, y + 0);
        tmp2 = getTileSafely(x + 0, y - 1);

        if(tmp1 && tmp1->navigable && tmp2 && tmp2->navigable)
        {
            diagonal.push_back(getTile(x - 1, y - 1));
        }

        tmp1 = getTileSafely(x + 1, y + 0);
        tmp2 = getTileSafely(x + 0, y - 1);

        if(tmp1 && tmp1->navigable && tmp2 && tmp2->navigable)
        {
            diagonal.push_back(getTile(x + 1, y - 1));
        }

        for(Tile *selected : diagonal)
        {
            if(selected && selected->navigate(head, 1.4f))
            {
                if(selected == start)
                {
                    return true;
                }
                else
                {
                    priority_insert(open_set, selected, finish, start);
                }
            }
        }
    }

    return false;
}

bool PathBuilder::calculatePath(Tile *start, Tile *finish, std::vector<const Tile*> &path)
{
    bool result = calculatePath(start, finish);

    if(result)
    {
        for(const Tile *tile = start; tile; tile = tile->parent)
        {
            path.push_back(tile);
        }
    }

    return result;
}

void PathBuilder::reset()
{
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            getTile(x, y)->reset();
        }
    }
}