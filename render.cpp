#include "render.hpp"
#include "configuration.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>

static void _drawSquare(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x+w, y);
    glVertex2f(x+w, y+h);
    glVertex2f(x, y+h);
    glEnd();
}

static void _drawFilledSquare(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(x, y);
    glVertex2f(x+w, y);
    glVertex2f(x, y+h);
    glVertex2f(x+w, y+h);
    glEnd();
}

void RenderGrid(const Grid<bool> &grid)
{
    for(size_t x = 0; x < grid.w; x++)
    {
        for(size_t y = 0; y < grid.h; y++)
        {
            if(grid.getTile(x, y))
            {
                _drawFilledSquare(x * TILE_W, y * TILE_H, TILE_W, TILE_H);
            }
            _drawSquare(x * TILE_W, y * TILE_H, TILE_W, TILE_H);
        }
    }
}

void RenderCursor(const Grid<bool> &grid)
{
    int x;
    int y;

    SDL_GetMouseState(&x, &y);

    x /= TILE_W;
    y /= TILE_H;

    if(x >= (int)grid.w || y >= (int)grid.h)
    {
        return;
    }

    x *= TILE_W;
    y *= TILE_H;

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(TILE_W, TILE_H);
    glVertex2f(TILE_W, 0);
    glVertex2f(0, TILE_H);
    glEnd();
    glPopMatrix();
}

void RenderShape(const std::vector<glm::vec2> &shape)
{
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    for(const glm::vec2 &point : shape)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPointSize(5.0f);

    glBegin(GL_POINTS);
    for(const glm::vec2 &point : shape)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}