#include "Agent.h"
#include <climits>
#include <cstdlib>
#include <queue>
#include <utility>
#include "World.h"

using namespace std;

int toIndex(Point2D p, int side)
{
    int half = side / 2;
    //position into flat array
    return (p.y + half) * side + p.x + half;
}

Point2D toPoint(int index, int side)
{
    int half = side / 2;
    //turn index into point
    return {index % side - half, index / side - half};
}

static int borderDistance(Point2D p, int side)
{
    int half = side / 2;
    //a* heustistic to estimate moves left to border
    return half - max(abs(p.x), abs(p.y));
}

std::vector<Point2D> Agent::generatePath(CatWorld* w)
{
    return generatePath(w, w->getCat());
}

std::vector<Point2D> Agent::generatePath(CatWorld* w, Point2D potentialBlock)
{
    int side = w->getWorldSideSize();
    int cells = side * side;

    int potentialBlockIndex = toIndex(potentialBlock, side);

    vector<int> cost(cells, INT_MAX); //size, value; defaults
    vector<int> cameFrom(cells, -1);
    vector<bool> closed(cells, false);

    //avoid point2d comparison issues with pair and min heap
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> frontier;

    Point2D cat = w->getCat();
    int start = toIndex(cat, side);
    cost[start] = 0;

    //setup frontier
    frontier.push( { borderDistance(cat, side), start } );

    //run pathfind
    int goal = -1;
    while (!frontier.empty())
    {
        int current = frontier.top().second;
        frontier.pop();

        if (closed[current])
            continue;

        closed[current] = true;

        Point2D pos = toPoint(current, side);
        if (w->catWinsOnSpace(pos))
        {
            goal = current;
            break;
        }

        Point2D next[6] = {CatWorld::NE(pos), CatWorld::NW(pos), CatWorld::E(pos), CatWorld::W(pos), CatWorld::SW(pos), CatWorld::SE(pos)};

        for (const Point2D& n : next)
        {
            int ni = toIndex(n, side);

            if (closed[ni] || ni == potentialBlockIndex || w->getContent(n))
                continue;

            int newCost = cost[current] + 1;

            if (newCost < cost[ni])
            {
                cost[ni] = newCost;
                cameFrom[ni] = current;

                frontier.push( { newCost + borderDistance(n, side), ni } );
            }
        }
    }

    //couldn't find anything,
    if (goal == -1)
        return vector<Point2D>();

    //found, build path
    vector<Point2D> path;
    for (int i = goal; i != start; i = cameFrom[i])
        path.push_back(toPoint(i, side));



    return path;
}

//emenate from starting point like BFS but trying to create enclosed shapes
void Agent::flood(CatWorld* w, bool fromBorder, vector<int>& dist, vector<double>& ways)
{
    int side = w->getWorldSideSize();
    int cells = side * side;

    //dist is moves from the source; ways is how many shortest paths reach cell
    dist.assign(cells, INT_MAX);
    ways.assign(cells, 0.0);

    static vector<int> frontier;
    frontier.clear();
    size_t head = 0;

    //source open cell or cat
    if (fromBorder)
    {
        for (int i = 0; i < cells; i++)
        {
            Point2D p = toPoint(i, side);

            if (w->catWinsOnSpace(p) && !w->getContent(p))
            {
                dist[i] = 0;
                ways[i] = 1.0;
                frontier.push_back(i);
            }
        }
    }
    else
    {
        int start = toIndex(w->getCat(), side);
        dist[start] = 0;
        ways[start] = 1.0;
        frontier.push_back(start);
    }

    while (head < frontier.size())
    {
        int current = frontier[head++];

        Point2D pos = toPoint(current, side);

        if (!fromBorder && w->catWinsOnSpace(pos))
            continue;

        Point2D next[6] = {CatWorld::NE(pos), CatWorld::NW(pos), CatWorld::E(pos), CatWorld::W(pos), CatWorld::SW(pos), CatWorld::SE(pos)};

        for (const Point2D& n : next)
        {
            if (!w->isValidPosition(n) || w->getContent(n))
                continue;

            int ni = toIndex(n, side);

            if (dist[ni] == INT_MAX)
            {
                dist[ni] = dist[current] + 1;
                frontier.push_back(ni);
            }

            if (dist[ni] == dist[current] + 1)
                ways[ni] += ways[current];
        }
    }
}
