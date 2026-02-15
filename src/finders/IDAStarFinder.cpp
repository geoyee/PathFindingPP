#include "pathfinding/finders/IDAStarFinder.h"

#include <chrono>
#include <cmath>
#include <limits>

namespace pathfinding
{
constexpr double SQRT2 = 1.4142135623730951;

IDAStarFinder::IDAStarFinder() : Finder()
{
    initHeuristic();
}

IDAStarFinder::IDAStarFinder(const FinderOptions& options) : Finder(options)
{
    initHeuristic();
}

void IDAStarFinder::initHeuristic()
{
    if (options_.diagonalMovement == DiagonalMovement::Never)
    {
        heuristic_ = heuristic::manhattan;
    }
    else
    {
        heuristic_ = heuristic::octile;
    }
}

util::Path IDAStarFinder::findPath(int startX, int startY, int endX, int endY, Grid& grid)
{
    Node *startNode = grid.getNodeAt(startX, startY);
    Node *endNode = grid.getNodeAt(endX, endY);

    auto h = [this, endNode](Node *a) { return heuristic_(std::abs(endNode->x - a->x), std::abs(endNode->y - a->y)); };

    double cutoff = h(startNode) * options_.weight;

    auto startTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count();

    while (true)
    {
        util::Path route;
        double t = search(startNode, 0, cutoff, route, 0, endNode, grid, startTime);

        if (std::isinf(t))
        {
            return {};
        }

        if (!route.empty())
        {
            return route;
        }

        cutoff = t;
    }
}

double IDAStarFinder::search(
    Node *node, double g, double cutoff, util::Path& route, int depth, Node *endNode, Grid& grid, long long startTime)
{
    if (timeLimit_ > 0)
    {
        auto now =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
                .count();
        if (now - startTime > static_cast<long long>(timeLimit_ * 1000))
        {
            return std::numeric_limits<double>::infinity();
        }
    }

    double h = heuristic_(std::abs(endNode->x - node->x), std::abs(endNode->y - node->y));
    double f = g + h * options_.weight;

    if (f > cutoff)
    {
        return f;
    }

    if (node == endNode)
    {
        route.resize(depth + 1);
        route[depth] = {node->x, node->y};
        return -1.0;
    }

    double min = std::numeric_limits<double>::infinity();
    auto neighbors = grid.getNeighbors(node, options_.diagonalMovement);

    for (Node *neighbor : neighbors)
    {
        if (trackRecursion_)
        {
            neighbor->retainCount++;
            if (!neighbor->tested)
            {
                neighbor->tested = true;
            }
        }

        double cost = (neighbor->x == node->x || neighbor->y == node->y) ? 1.0 : SQRT2;
        double t = search(neighbor, g + cost, cutoff, route, depth + 1, endNode, grid, startTime);

        if (t < 0)
        {
            route[depth] = {node->x, node->y};
            return t;
        }

        if (trackRecursion_)
        {
            neighbor->retainCount--;
            if (neighbor->retainCount == 0)
            {
                neighbor->tested = false;
            }
        }

        if (t < min)
        {
            min = t;
        }
    }

    return min;
}
} // namespace pathfinding
