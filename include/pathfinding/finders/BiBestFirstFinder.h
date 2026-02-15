#ifndef PATHFINDING_FINDERS_BI_BEST_FIRST_FINDER_H
#define PATHFINDING_FINDERS_BI_BEST_FIRST_FINDER_H

#include "pathfinding/finders/BiAStarFinder.h"

namespace pathfinding
{
class PATHFINDING_API BiBestFirstFinder : public BiAStarFinder
{
public:
    BiBestFirstFinder();
    explicit BiBestFirstFinder(const FinderOptions& options);
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_BI_BEST_FIRST_FINDER_H
