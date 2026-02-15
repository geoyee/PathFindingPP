#include "pathfinding/finders/BiDijkstraFinder.h"

namespace pathfinding
{
BiDijkstraFinder::BiDijkstraFinder() : BiAStarFinder()
{
    heuristic_ = [](double, double) { return 0.0; };
}

BiDijkstraFinder::BiDijkstraFinder(const FinderOptions& options) : BiAStarFinder(options)
{
    heuristic_ = [](double, double) { return 0.0; };
}
} // namespace pathfinding
