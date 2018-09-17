#include "marching.hpp"
#include "configuration.hpp"
#include <iostream>
#include <queue>

static const Marching::Solution solutions[16] = {
    Marching::Solution::None,
    Marching::Solution::Up,
    Marching::Solution::Right,
    Marching::Solution::Right,
    Marching::Solution::Left,
    Marching::Solution::Up,
    Marching::Solution::Left,
    Marching::Solution::Right,
    Marching::Solution::Down,
    Marching::Solution::Up,
    Marching::Solution::Down,
    Marching::Solution::Down,
    Marching::Solution::Left,
    Marching::Solution::Up,
    Marching::Solution::Left,
    Marching::Solution::None
};

static const glm::uvec2 offsets[16] = {
    { 0, 0 },
    { 0, 0 },
    { 1, 0 },
    { 0, 0 },
    { 0, 1 },
    { 0, 0 },
    { 0, 1 },
    { 0, 0 },
    { 1, 1 },
    { 0, 0 },
    { 1, 0 },
    { 0, 0 },
    { 0, 1 },
    { 0, 0 },
    { 1, 0 },
    { 0, 0 }
};

static bool coordInBounds(const Grid<bool> &grid, size_t x, size_t y)
{
    return x < grid.w && y < grid.h;
}

static bool safeGetTile(const Grid<bool> &grid, size_t x, size_t y)
{
    return coordInBounds(grid, x, y) && grid.getTile(x, y);
}

static void fillGrid(Grid<bool> &grid, size_t x, size_t y)
{
    std::queue<glm::uvec2> open_set;
    open_set.emplace(x, y);

    while(!open_set.empty())
    {
        const glm::uvec2 &current = open_set.front();

        grid.getTile(current.x, current.y) = false;

        const glm::uvec2 neighbours[] = {
                { current.x - 1, current.y },
                { current.x + 1, current.y },
                { current.x, current.y - 1 },
                { current.x, current.y + 1 }
        };

        for(const glm::uvec2 &neighbour : neighbours)
        {
            if(safeGetTile(grid, neighbour.x, neighbour.y))
            {
                open_set.emplace(neighbour.x, neighbour.y);
            }
        }
        open_set.pop();
    }
}

static size_t getIndex(const Grid<bool> &grid, size_t x, size_t y)
{
    const bool topLeft = safeGetTile(grid, x, y);
    const bool topRight = safeGetTile(grid, x + 1, y);
    const bool bottomLeft = safeGetTile(grid, x, y + 1);
    const bool bottomRight = safeGetTile(grid, x + 1, y + 1);

    size_t index = 0;

    if(topLeft)
    {
        index += 1;
    }

    if(topRight)
    {
        index += 2;
    }

    if(bottomLeft)
    {
        index += 4;
    }

    if(bottomRight)
    {
        index += 8;
    }

    return index;
}

static Marching::Solution solveTile(const Grid<bool> &grid, size_t x, size_t y, Marching::Solution previousSolution)
{
    size_t index = getIndex(grid, x, y);

    if(index == 6 && previousSolution != Marching::Solution::Up)
    {
        return Marching::Solution::Right;
    }

    if(index == 9 && previousSolution != Marching::Solution::Right)
    {
        return Marching::Solution::Down;
    }

    return solutions[index];
}

Shape Marching::solveShape(const Grid<bool> &grid, size_t x, size_t y)
{
    Shape result;

    size_t current_x = x;
    size_t current_y = y;
    
    Marching::Solution previousSolution = Marching::Solution::None;

    while(true)
    {
        const Marching::Solution solution = solveTile(grid, current_x, current_y, previousSolution);

        if(previousSolution != solution)
        {
            const float position_x = current_x * TILE_W + TILE_W;
            const float position_y = current_y * TILE_H + TILE_H;

            result.points.emplace_back(position_x, position_y);
            previousSolution = solution;
        }

        switch(solution)
        {
            case Marching::Solution::Up:
                current_y--;
                break;
            case Marching::Solution::Down:
                current_y++;
                break;
            case Marching::Solution::Left:
                current_x--;
                break;
            case Marching::Solution::Right:
                current_x++;
                break;
        }

        if(current_x == x && current_y == y)
        {
            result.points.emplace_back(result.points[0]);
            break;
        }
    }

    return result;
}

std::vector<Shape> Marching::solveGrid(const Grid<bool> &grid)
{
    Grid<bool> copy = grid;
    std::vector<Shape> result;

    Marching::Solution solution = Marching::Solution::None;

    for(int y = -1; y < (int)grid.h - 1; y++)
    {
        for(int x = -1; x < (int)grid.w - 1; x++)
        {
            solution = solveTile(copy, x, y, solution);

            if(solution != Marching::Solution::None)
            {
                const glm::uvec2 &offset = offsets[getIndex(copy, x, y)];
                const Shape shape = Marching::solveShape(copy, x, y);
                fillGrid(copy, (size_t)x+1, (size_t)y+1);
                result.emplace_back(shape);
            }
        }
    }

    return result;
}