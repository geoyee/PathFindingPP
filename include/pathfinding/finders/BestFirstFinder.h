#ifndef PATHFINDING_FINDERS_BEST_FIRST_FINDER_H
#define PATHFINDING_FINDERS_BEST_FIRST_FINDER_H

#include "pathfinding/finders/AStarFinder.h"

namespace pathfinding
{
class PATHFINDING_API BestFirstFinder : public AStarFinder
{
public:
    BestFirstFinder();
    explicit BestFirstFinder(const FinderOptions& options);
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BEST_FIRST_FINDER_H
