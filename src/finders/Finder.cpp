#include "pathfinding/finders/Finder.h"

namespace pathfinding
{
Finder::Finder()
{
    initDiagonalMovement();
}

Finder::Finder(const FinderOptions& options) : options_(options)
{
    initDiagonalMovement();
}

void Finder::initDiagonalMovement()
{
    if (options_.diagonalMovement == DiagonalMovement::Never)
    {
        if (options_.allowDiagonal)
        {
            if (options_.dontCrossCorners)
            {
                options_.diagonalMovement = DiagonalMovement::OnlyWhenNoObstacles;
            }
            else
            {
                options_.diagonalMovement = DiagonalMovement::IfAtMostOneObstacle;
            }
        }
        else
        {
            options_.diagonalMovement = DiagonalMovement::Never;
        }
    }
}
} // namespace pathfinding
