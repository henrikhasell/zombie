#ifndef ZOMBIE_GAME_NAVIGATION_HPP
#define ZOMBIE_GAME_NAVIGATION_HPP

#include <vector>
#include "grid.hpp"

namespace Navigation
{
    struct Tile
    {
        void initialise(int x, int y, bool navigable);
        bool navigate(const Tile *parent, float cost);
        void reset();
        float heuristic(const Tile *start, const Tile *destination) const;

        float cost;
        bool navigable;
        const Tile *parent;
        int x;
        int y;
    };

    class PathBuilder
    {
    private:
        Tile *tiles;
    public:
        const int width;
        const int height;
        PathBuilder(const Grid<bool> &grid);
        ~PathBuilder();
        Tile *getTile(int x, int y);
        Tile *getTileSafely(int x, int y);
        void calculatePath(Tile *finish);
        bool calculatePath(Tile *start, std::vector<const Tile*> &path);
        void reset();
    };
}

#endif
