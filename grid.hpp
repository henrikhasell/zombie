#ifndef GRID_HPP
#define GRID_HPP

#include <cstdlib>

template<class T> class Grid
{
public:
    const int w;
    const int h;

    Grid(int w, int h) : w(w), h(h)
    {
        data = new T[w * h]();
    }

    Grid(size_t w, size_t h, const T *data) : Grid(w, h)
    {
        for(size_t i = 0; i < w * h; i++)
        {
            this->data[i] = data[i];
        }
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

    T &getTile(int x, int y) const
    {
        return data[x + y * w];
    }
private:
    T *data;
};

#endif