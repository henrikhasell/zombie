#ifndef FONT_HPP
#define FONT_HPP

#include <SDL2/SDL_ttf.h>
#include "texture.hpp"

class Font
{
public:
    Font();
    ~Font();
    bool load(const char path[], int size);
    void renderFormat(Texture &texture, const char format[], ...);
    void renderString(Texture &texture, const char string[]);
private:
    TTF_Font *font;
};


#endif