#ifndef PATHFINDING_CORE_DIAGONAL_MOVEMENT_H
#define PATHFINDING_CORE_DIAGONAL_MOVEMENT_H

namespace pathfinding
{
enum class DiagonalMovement
{
    Always = 1,
    Never = 2,
    IfAtMostOneObstacle = 3,
    OnlyWhenNoObstacles = 4
};
} // namespace pathfinding

#endif // !PATHFINDING_CORE_DIAGONAL_MOVEMENT_H
