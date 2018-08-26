#include "render.hpp"
#include "configuration.hpp"
#include <cmath>
#include <GL/glew.h>
#include <SDL2/SDL.h>

static void _drawCircle(GLfloat x, GLfloat y, GLfloat r)
{
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for(GLfloat i = 0.0f; i < M_PI * 2; i += 0.2f)
    {
        glVertex2f(x + cosf(i) * r, y + sinf(i) * r);
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y - r);
    glEnd();
}

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

void UpdateProjection(const Camera &camera)
{
    glViewport(0, 0, camera.w, camera.h);

    const float w = camera.w / camera.zoom;
    const float h = camera.h / camera.zoom;

    glMatrixMode(GL_PROJECTION);
    glOrtho(camera.x, camera.x + w, camera.y + h, camera.y, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

void RenderCursor(const Grid<bool> &grid, const Camera &camera)
{
    int x;
    int y;

    SDL_GetMouseState(&x, &y);

    x += camera.x * camera.zoom;
    y += camera.y * camera.zoom;

    x /= (TILE_W * camera.zoom);
    y /= (TILE_H * camera.zoom);

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

    glBegin(GL_LINE_STRIP);
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

void RenderPlayer(const b2Body &body)
{
    const b2Vec2 &position = body.GetPosition();
    const float32 rotation = body.GetAngle();
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(rotation * 57.2958f, 0.0f, 0.0f, 1.0f);
    _drawCircle(0.0f, 0.0f, 1.0f);
    glPopMatrix();
}