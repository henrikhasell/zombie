#include "font.hpp"
#include "render.hpp"
#include "configuration.hpp"
#include <cmath>
#include <GL/glew.h>
#include <SDL2/SDL.h>

static void _drawCircle(GLfloat x, GLfloat y, GLfloat r)
{
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
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x+w, y);
    glVertex2f(x+w, y+h);
    glVertex2f(x, y+h);
    glEnd();
}

static void _drawFilledSquare(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x+w, y);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x+w, y+h);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y+h);
    glEnd();
}

static Font *font;

void InitialiseRenderer()
{
    font = new Font();
    font->load("NanumGothicCoding-Regular.ttf", 32);
}

void CleanupRenderer()
{
    delete font;
}


void UpdateProjection(const Camera &camera)
{
    glViewport(0, 0, camera.w, camera.h);

    const float w = camera.w / camera.zoom;
    const float h = camera.h / camera.zoom;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(camera.x, camera.x + w, camera.y + h, camera.y, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderGrid(const Grid<bool> &grid)
{
    glColor3f(0.2f, 0.2f, 0.2f);

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

    if(x >= grid.w || y >= grid.h)
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
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(rotation * 57.2958f, 0.0f, 0.0f, 1.0f);
    _drawCircle(0.0f, 0.0f, 1.0f);
    glPopMatrix();
}

void RenderBullet(const b2Body &body)
{
    const b2Vec2 &position = body.GetPosition();
    const float32 rotation = body.GetAngle();
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(rotation * 57.2958f, 0.0f, 0.0f, 1.0f);
    _drawCircle(0.0f, 0.0f, 0.25f);
    glPopMatrix();
}

void RenderZombie(const b2Body &body)
{
    constexpr float w = 2.0f;
    RenderPlayer(body);
    const b2Vec2 &position = body.GetPosition();
    Zombie *zombie = (Zombie*)body.GetUserData();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(position.x - w / 2.0f, position.y - 2.05f, 0.0f);
    glScalef(zombie->hitpoints / zombie->max_hitpoints, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(w, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(w, 0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(position.x - w / 2.0f, position.y - 2.05f);
    glVertex2f(position.x + w / 2.0f, position.y - 2.05f);
    glVertex2f(position.x + w / 2.0f, position.y - 1.55f);
    glVertex2f(position.x - w / 2.0f, position.y - 1.55f);
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    for(const b2Vec2 &point : zombie->path)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void RenderText(GLfloat x, GLfloat y, const char format[], ...)
{
    va_list a1, a2;
    va_start(a1, format);
    va_copy(a2, a1);

    size_t size = (size_t)vsnprintf(nullptr, 0, format, a1) + 1;
    char *buffer = new char[size];
    vsnprintf(buffer, size, format, a2);
    va_end(a1);
    va_end(a2);
    Texture texture;
    font->renderString(texture, buffer);
    texture.bind();
    delete[] buffer;
    glColor3f(1.0f, 1.0f, 1.0f);
    _drawFilledSquare(x, y, (GLfloat)texture.w / 10.0f, (GLfloat)texture.h / 10.0f);
}