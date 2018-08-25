#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <glm/vec2.hpp>
#include "grid.hpp"

void RenderGrid(const Grid<bool> &grid);

void RenderCursor(const Grid<bool> &grid);

void RenderShape(const std::vector<glm::vec2> &shape);

#endif