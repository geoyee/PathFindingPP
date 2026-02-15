#ifndef PATHFINDING_CORE_GRID_H
#define PATHFINDING_CORE_GRID_H

#include "pathfinding/core/DiagonalMovement.h"
#include "pathfinding/core/Node.h"

#include <vector>
#include <memory>

namespace pathfinding
{
class Grid
{
public:
    int width;
    int height;

    Grid(int width, int height);
    Grid(const std::vector<std::vector<int>>& matrix);

    Node *getNodeAt(int x, int y);
    const Node *getNodeAt(int x, int y) const;
    bool isWalkableAt(int x, int y) const;
    bool isInside(int x, int y) const;
    void setWalkableAt(int x, int y, bool walkable);
    std::vector<Node *> getNeighbors(Node *node, DiagonalMovement diagonalMovement);
    Grid clone() const;
    void resetNodes();

private:
    void buildNodes(int width, int height);
    void buildNodesFromMatrix(const std::vector<std::vector<int>>& matrix);

    std::vector<std::vector<std::unique_ptr<Node>>> nodes_;
};
} // namespace pathfinding

#endif // !PATHFINDING_CORE_GRID_H
