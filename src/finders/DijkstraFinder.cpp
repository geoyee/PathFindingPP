#include "pathfinding/finders/DijkstraFinder.h"

namespace pathfinding
{
DijkstraFinder::DijkstraFinder() : AStarFinder()
{
    heuristic_ = [](double, double) { return 0.0; };
}

DijkstraFinder::DijkstraFinder(const FinderOptions& options) : AStarFinder(options)
{
    heuristic_ = [](double, double) { return 0.0; };
}
} // namespace pathfinding
