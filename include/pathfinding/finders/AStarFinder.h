#ifndef PATHFINDING_FINDERS_A_STAR_FINDER_H
#define PATHFINDING_FINDERS_A_STAR_FINDER_H

#include "pathfinding/finders/Finder.h"
#include "pathfinding/core/Heuristic.hpp"

#include <functional>
#include <vector>

namespace pathfinding
{
class AStarFinder : public Finder
{
public:
    AStarFinder();
    explicit AStarFinder(const FinderOptions& options);

    Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;

protected:
    void initHeuristic();

    Heuristic::HeuristicFunc heuristic_;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_A_STAR_FINDER_H
