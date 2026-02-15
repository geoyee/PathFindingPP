#include "pathfinding/finders/BiBestFirstFinder.h"

namespace pathfinding
{
BiBestFirstFinder::BiBestFirstFinder() : BiAStarFinder()
{
    auto orig = heuristic_;
    heuristic_ = [orig](double dx, double dy) { return orig(dx, dy) * 1000000.0; };
}

BiBestFirstFinder::BiBestFirstFinder(const FinderOptions& options) : BiAStarFinder(options)
{
    auto orig = heuristic_;
    heuristic_ = [orig](double dx, double dy) { return orig(dx, dy) * 1000000.0; };
}
} // namespace pathfinding
