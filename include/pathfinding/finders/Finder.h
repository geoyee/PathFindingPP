#ifndef PATHFINDING_FINDERS_FINDER_H
#define PATHFINDING_FINDERS_FINDER_H

#include "pathfinding/core/DiagonalMovement.h"
#include "pathfinding/core/Export.hpp"
#include "pathfinding/core/Grid.h"
#include "pathfinding/core/Util.h"

#include <utility>
#include <vector>

namespace pathfinding
{
struct FinderOptions
{
    bool allowDiagonal = false;
    bool dontCrossCorners = false;
    DiagonalMovement diagonalMovement = DiagonalMovement::Never;
    double weight = 1.0;
};

class PATHFINDING_API Finder
{
public:
    Finder();
    explicit Finder(const FinderOptions& options);
    virtual ~Finder() = default;

    virtual Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) = 0;

protected:
    void initDiagonalMovement();

    FinderOptions options_;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_FINDER_H
