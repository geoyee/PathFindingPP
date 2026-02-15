#ifndef PATHFINDING_FINDERS_BI_DIJKSTRA_FINDER_H
#define PATHFINDING_FINDERS_BI_DIJKSTRA_FINDER_H

#include "pathfinding/finders/BiAStarFinder.h"

namespace pathfinding
{
class PATHFINDING_API BiDijkstraFinder : public BiAStarFinder
{
public:
    BiDijkstraFinder();
    explicit BiDijkstraFinder(const FinderOptions& options);
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BI_DIJKSTRA_FINDER_H
