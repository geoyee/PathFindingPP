#ifndef PATHFINDING_FINDERS_DIJKSTRA_FINDER_H
#define PATHFINDING_FINDERS_DIJKSTRA_FINDER_H

#include "pathfinding/finders/AStarFinder.h"

namespace pathfinding
{
class DijkstraFinder : public AStarFinder
{
public:
    DijkstraFinder();
    explicit DijkstraFinder(const FinderOptions& options);
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_DIJKSTRA_FINDER_H
