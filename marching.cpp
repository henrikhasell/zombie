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

static bool coordInBounds(const Grid<bool> &grid, int x, int y)
{
    return x >= 0 && y >= 0 && x < grid.w && y < grid.h;
}

static bool safeGetTile(const Grid<bool> &grid, int x, int y)
{
    return coordInBounds(grid, x, y) && grid.getTile(x, y);
}

static size_t getIndex(const Grid<bool> &grid, int x, int y)
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

static Marching::Solution solveTile(const Grid<bool> &grid, int x, int y, Marching::Solution previousSolution)
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

Shape Marching::solveShape(const Grid<bool> &grid, Grid<bool> &visited, int x, int y)
{
    Shape result;

    int current_x = x;
    int current_y = y;
    
    Marching::Solution previousSolution = Marching::Solution::None;

    while(true)
    {
        const Marching::Solution solution = solveTile(grid, current_x, current_y, previousSolution);

        if(previousSolution != solution)
        {
            previousSolution = solution;

            const float position_x = (current_x + 1) * TILE_W;
            const float position_y = (current_y + 1) * TILE_H;

            result.points.emplace_back(position_x, position_y);
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

	    visited.getTile(current_x, current_y) = true;
    }

    return result;
}

std::vector<Shape> Marching::solveGrid(const Grid<bool> &grid)
{
    Grid<bool> copy = grid;
    Grid<bool> visited(grid.w, grid.h);
    std::vector<Shape> result;

    Marching::Solution solution = Marching::Solution::None;

    for(int y = 0; y < grid.h - 1; y++)
    {
        for(int x = 0; x < grid.w - 1; x++)
        {
            if(visited.getTile(x, y))
            {
                continue;
            }

            solution = solveTile(copy, x, y, solution);

            if(solution != Marching::Solution::None)
            {
                const Shape shape = Marching::solveShape(copy, visited, x, y);
                result.emplace_back(shape);
            }
        }
    }

    return result;
}
