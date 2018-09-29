#include "font.hpp"
#include <iostream>

Font::Font() : font(nullptr)
{

}

Font::~Font()
{
    if(font)
    {
        TTF_CloseFont(font);
    }
}

bool Font::load(const char path[], int size)
{
    if(font)
    {
        TTF_CloseFont(font);
    }

    font = TTF_OpenFont(path, size);

    if(font == nullptr)
    {
        std::cerr << "Failed to load " << path << ": " << TTF_GetError() << std::endl;
    }

    return font != nullptr;
}


void Font::renderFormat(Texture &texture, const char format[], ...)
{
    va_list a1, a2;
    va_start(a1, format);
    va_copy(a2, a1);
    size_t bufferSize = (size_t)vsnprintf(nullptr, 0, format, a1) + 1;
    char *buffer = new char[bufferSize];
    vsnprintf(buffer, bufferSize, format, a2);
    renderString(texture, buffer);
    delete[] buffer;
    va_end(a1);
    va_end(a2);
}

void Font::renderString(Texture &texture, const char string[])
{
    SDL_Colour colour;

    colour.r = 0xff;
    colour.g = 0xff;
    colour.b = 0xff;
    colour.a = 0xff;

    SDL_Surface *surface = TTF_RenderText_Blended(font, string, colour);

    if(surface)
    {
        texture.load(surface);
        SDL_FreeSurface(surface);
    }
}