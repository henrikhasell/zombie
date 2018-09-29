#ifndef MARCHING_HPP
#define MARCHING_HPP

#include <glm/vec2.hpp>
#include <vector>
#include "shape.hpp"
#include "grid.hpp"

namespace Marching
{
    enum Solution
    {
        None, Up, Down, Left, Right
    };

    Shape solveShape(const Grid<bool> &grid, int x, int y);

    std::vector<Shape> solveGrid(const Grid<bool> &grid);
}

#endif