#include "Catcher.h"
#include <climits>
#include <cstdlib>
#include "World.h"

Point2D Catcher::Move(CatWorld* world)
{
    auto cat = world->getCat();
    auto path = generatePath(world);

    //contingency, fill out around cat if no path found
    if (path.empty())
    {
        for (auto n : CatWorld::neighbors(cat))
            if (!world->getContent(n))
                return n;

        return cat;
    }

    //path not empty; find best position to block cat
    int currentLength = static_cast<int>(path.size());
    Point2D best = path.back();

    int bestLength = -1;
    int bestDistance = INT_MAX;

    //try each cell on cat path, block longest after or closest if equal length
    for (int i = currentLength - 1; i >= 0; i--)
    {
        Point2D p = path[i];

        auto after = generatePath(world, p);

        int length = after.empty() ? INT_MAX : static_cast<int>(after.size());
        int distance = abs(p.x - cat.x) + abs(p.y - cat.y);

        if (length > bestLength || (length == bestLength && distance < bestDistance))
        {
            bestLength = length;
            bestDistance = distance;
            best = p;
        }
    }

    if (bestLength > currentLength)
        return best;

    //no block lengthens the path, so just crowd the cat
    bestDistance = INT_MAX;

    for (auto n : CatWorld::neighbors(cat))
    {
        int distance = abs(n.x - cat.x) + abs(n.y - cat.y);

        if (!world->getContent(n) && distance < bestDistance)
        {
            bestDistance = distance;
            best = n;
        }
    }

    return best;
}
