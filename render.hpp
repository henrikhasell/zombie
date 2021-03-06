#ifndef RENDER_HPP
#define RENDER_HPP

#include <Box2D/Box2D.h>
#include <vector>
#include <glm/vec2.hpp>
#include "player.hpp"
#include "camera.hpp"
#include "grid.hpp"

void InitialiseRenderer();

void CleanupRenderer();

void UpdateProjection(const Camera &camera);

void RenderGrid(const Grid<bool> &grid);

void RenderCursor(const Grid<bool> &grid, const Camera &camera);

void RenderPathBuilder(Navigation::PathBuilder &pathBuilder);

void RenderShape(const std::vector<glm::vec2> &shape);

void RenderPlayer(const b2Body &body);

void RenderBullet(const b2Body &body);

void RenderZombie(const b2Body &body);

void RenderPath(const std::vector<b2Vec2> &path);

void RenderText(GLfloat x, GLfloat y, const char format[], ...);

#endif