#include "texture.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#   define R_MASK 0xFF000000
#   define G_MASK 0x00FF0000
#   define B_MASK 0x0000FF00
#   define A_MASK 0x000000FF
#else
#   define R_MASK 0x000000FF
#   define G_MASK 0x0000FF00
#   define B_MASK 0x00FF0000
#   define A_MASK 0xFF000000
#endif

Texture::Texture() : w(2), h(2)
{
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    const GLubyte pixels[] = {
            0xff, 0xff, 0xff, 0xff,
            0xaa, 0xaa, 0xaa, 0xff,
            0xaa, 0xaa, 0xaa, 0xff,
            0xff, 0xff, 0xff, 0xff
    };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 2,
                 2, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

bool Texture::load(const char path[], bool gamma_correction)
{
    bool result = false;

    SDL_Surface *surface = IMG_Load(path);

    if(surface)
    {
        result = load(surface, gamma_correction);
        SDL_FreeSurface(surface);
    }

    if(!result)
    {
        std::cerr << "Failed to load: " << path << std::endl;
    }

    return result;
}

bool Texture::load(SDL_Surface *surface, bool gamma_correction)
{
    SDL_Surface *optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    if(optimised)
    {
        SDL_FillRect(optimised, NULL, 0x000000ff);
        SDL_SetSurfaceAlphaMod(surface, 0xFF);
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

        SDL_BlitSurface(surface, NULL, optimised, NULL);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0,
             gamma_correction ? GL_SRGB_ALPHA : GL_RGBA,
             optimised->w,
             optimised->h, 0,
             GL_RGBA, GL_UNSIGNED_BYTE,
             optimised->pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        w = optimised->w;
        h = optimised->h;

        SDL_FreeSurface(optimised);
    }

    return optimised != NULL;
}

void Texture::bind(GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

#undef R_MASK
#undef G_MASK
#undef B_MASK
#undef A_MASK