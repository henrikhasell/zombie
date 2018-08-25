#ifndef GRID_HPP
#define GRID_HPP

#include <cstdlib>

template<class T> class Grid
{
public:
    const size_t w;
    const size_t h;

    Grid(size_t w, size_t h) : w(w), h(h)
    {
        data = new T[w * h]();
    }

    Grid(const Grid &grid) : Grid(grid.w, grid.h)
    {
        for(size_t i = 0; i < w * h; i++)
        {
            data[i] = grid.data[i];
        }
    }

    ~Grid()
    {
        delete[] data;
    }

    T &getTile(size_t x, size_t y) const
    {
        return data[x + y * w];
    }
private:
    T *data;
};

#endif