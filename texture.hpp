#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>

struct Texture
{
    Texture();
    ~Texture();
    bool load(SDL_Surface *surface, bool gamma_correction = true);
    bool load(const char path[], bool gamma_correction);
    void bind(GLenum texture = GL_TEXTURE0) const;

    GLuint texture;
    int w;
    int h;
};

#endif // TEXTURE_HPP