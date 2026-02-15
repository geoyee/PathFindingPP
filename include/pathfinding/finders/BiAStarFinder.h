#ifndef PATHFINDING_FINDERS_BI_A_STAR_FINDER_H
#define PATHFINDING_FINDERS_BI_A_STAR_FINDER_H

#include "pathfinding/finders/Finder.h"
#include "pathfinding/core/Heuristic.hpp"

#include <functional>
#include <vector>

namespace pathfinding
{
class PATHFINDING_API BiAStarFinder : public Finder
{
public:
    BiAStarFinder();
    explicit BiAStarFinder(const FinderOptions& options);

    util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;

protected:
    void initHeuristic();

    heuristic::HeuristicFunc heuristic_;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BI_A_STAR_FINDER_H
