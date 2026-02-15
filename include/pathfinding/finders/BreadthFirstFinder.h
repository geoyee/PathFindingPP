#ifndef PATHFINDING_FINDERS_BREADTH_FIRST_FINDER_H
#define PATHFINDING_FINDERS_BREADTH_FIRST_FINDER_H

#include "pathfinding/finders/Finder.h"

#include <queue>

namespace pathfinding
{

class PATHFINDING_API BreadthFirstFinder : public Finder
{
public:
    BreadthFirstFinder();
    explicit BreadthFirstFinder(const FinderOptions& options);

    Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BREADTH_FIRST_FINDER_H
