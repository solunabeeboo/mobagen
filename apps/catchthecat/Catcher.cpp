#include "Catcher.h"
#include <climits>
#include <cstdlib>
#include "World.h"

using namespace std;

Point2D Catcher::Move(CatWorld* world)
{
    auto cat = world->getCat();
    int side = world->getWorldSideSize();
    int cells = side * side;

    //static so the buffers get reused instead of reallocated every turn
    static vector<int> catDist, borderDist;
    static vector<double> catWays, borderWays;

    //distance + shortest path count, from the cat and from the border
    flood(world, false, catDist, catWays);
    flood(world, true, borderDist, borderWays);

    //cats current distance to the closest border
    int shortest = borderDist[toIndex(cat, side)];

    //contingency, fill out around cat if no path found
    if (shortest == INT_MAX)
    {
        for (auto n : CatWorld::neighbors(cat))
            if (!world->getContent(n))
                return n;

        return cat;
    }

    static vector<int> ringSize;
    ringSize.assign(shortest, 0);
    for (int i = 0; i < cells; i++)
        if (catDist[i] > 0 && catDist[i] < shortest)
            ringSize[catDist[i]]++;

    //smallest ring we can finish before the cat reaches the border
    int ring = -1;
    for (int r = 1; r < shortest; r++)
        if (ringSize[r] <= shortest - 1 && (ring == -1 || ringSize[r] <= ringSize[ring]))
            ring = r;

    Point2D best = cat;
    double bestThrough = -1.0;
    int bestTouching = -1;

    //pick the cell on the ring
    for (int i = 0; i < cells; i++)
    {
        if (catDist[i] <= 0 || catDist[i] == INT_MAX || borderDist[i] == INT_MAX)
            continue;

        bool onPath = catDist[i] + borderDist[i] == shortest;
        double through = onPath ? catWays[i] * borderWays[i] : 0.0;

        if (ring != -1 ? catDist[i] != ring : !onPath)
            continue;

        Point2D p = toPoint(i, side);

        //blocked cells next to this one, on a tie prefer it so the ring stays connected
        int touching = 0;
        for (auto n : CatWorld::neighbors(p))
            if (world->isValidPosition(n) && world->getContent(n))
                touching++;

        if (through > bestThrough || (through == bestThrough && touching > bestTouching))
        {
            bestThrough = through;
            bestTouching = touching;
            best = p;
        }
    }

    if (best != cat)
        return best;

    //nothing picked, just crowd the cat
    for (auto n : CatWorld::neighbors(cat))
        if (!world->getContent(n))
            return n;

    return cat;
}
