#include "pathfinding/finders/BestFirstFinder.h"

namespace pathfinding
{
BestFirstFinder::BestFirstFinder() : AStarFinder()
{
    auto orig = heuristic_;
    heuristic_ = [orig](double dx, double dy) { return orig(dx, dy) * 1000000.0; };
}

BestFirstFinder::BestFirstFinder(const FinderOptions& options) : AStarFinder(options)
{
    auto orig = heuristic_;
    heuristic_ = [orig](double dx, double dy) { return orig(dx, dy) * 1000000.0; };
}
} // namespace pathfinding
