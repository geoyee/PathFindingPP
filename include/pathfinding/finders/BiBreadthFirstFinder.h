#ifndef PATHFINDING_FINDERS_BI_BREADTH_FIRST_FINDER_H
#define PATHFINDING_FINDERS_BI_BREADTH_FIRST_FINDER_H

#include "pathfinding/finders/Finder.h"

#include <queue>

namespace pathfinding
{
class PATHFINDING_API BiBreadthFirstFinder : public Finder
{
public:
    BiBreadthFirstFinder();
    explicit BiBreadthFirstFinder(const FinderOptions& options);

    Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;

private:
    static constexpr int BY_START = 0;
    static constexpr int BY_END = 1;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BI_BREADTH_FIRST_FINDER_H
