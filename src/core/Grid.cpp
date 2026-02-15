#include "pathfinding/core/Grid.h"

#include <stdexcept>

namespace pathfinding
{
Grid::Grid(int width, int height) : width(width), height(height)
{
    buildNodes(width, height);
}

Grid::Grid(const std::vector<std::vector<int>>& matrix)
{
    height = static_cast<int>(matrix.size());
    width = height > 0 ? static_cast<int>(matrix[0].size()) : 0;
    buildNodesFromMatrix(matrix);
}

void Grid::buildNodes(int width, int height)
{
    nodes_.resize(height);
    for (int i = 0; i < height; ++i)
    {
        nodes_[i].reserve(width);
        for (int j = 0; j < width; ++j)
        {
            nodes_[i].push_back(std::make_unique<Node>(j, i, true));
        }
    }
}

void Grid::buildNodesFromMatrix(const std::vector<std::vector<int>>& matrix)
{
    nodes_.resize(height);
    for (int i = 0; i < height; ++i)
    {
        nodes_[i].reserve(width);
        for (int j = 0; j < width; ++j)
        {
            bool walkable = (matrix[i][j] == 0);
            nodes_[i].push_back(std::make_unique<Node>(j, i, walkable));
        }
    }
}

Node *Grid::getNodeAt(int x, int y)
{
    return nodes_[y][x].get();
}

const Node *Grid::getNodeAt(int x, int y) const
{
    return nodes_[y][x].get();
}

bool Grid::isWalkableAt(int x, int y) const
{
    return isInside(x, y) && nodes_[y][x]->walkable;
}

bool Grid::isInside(int x, int y) const
{
    return (x >= 0 && x < width) && (y >= 0 && y < height);
}

void Grid::setWalkableAt(int x, int y, bool walkable)
{
    nodes_[y][x]->walkable = walkable;
}

std::vector<Node *> Grid::getNeighbors(Node *node, DiagonalMovement diagonalMovement)
{
    int x = node->x;
    int y = node->y;
    std::vector<Node *> neighbors;

    bool s0 = false, s1 = false, s2 = false, s3 = false;
    bool d0 = false, d1 = false, d2 = false, d3 = false;

    if (isWalkableAt(x, y - 1))
    {
        neighbors.push_back(nodes_[y - 1][x].get());
        s0 = true;
    }
    if (isWalkableAt(x + 1, y))
    {
        neighbors.push_back(nodes_[y][x + 1].get());
        s1 = true;
    }
    if (isWalkableAt(x, y + 1))
    {
        neighbors.push_back(nodes_[y + 1][x].get());
        s2 = true;
    }
    if (isWalkableAt(x - 1, y))
    {
        neighbors.push_back(nodes_[y][x - 1].get());
        s3 = true;
    }

    if (diagonalMovement == DiagonalMovement::Never)
    {
        return neighbors;
    }

    if (diagonalMovement == DiagonalMovement::OnlyWhenNoObstacles)
    {
        d0 = s3 && s0;
        d1 = s0 && s1;
        d2 = s1 && s2;
        d3 = s2 && s3;
    }
    else if (diagonalMovement == DiagonalMovement::IfAtMostOneObstacle)
    {
        d0 = s3 || s0;
        d1 = s0 || s1;
        d2 = s1 || s2;
        d3 = s2 || s3;
    }
    else if (diagonalMovement == DiagonalMovement::Always)
    {
        d0 = true;
        d1 = true;
        d2 = true;
        d3 = true;
    }

    if (d0 && isWalkableAt(x - 1, y - 1))
    {
        neighbors.push_back(nodes_[y - 1][x - 1].get());
    }
    if (d1 && isWalkableAt(x + 1, y - 1))
    {
        neighbors.push_back(nodes_[y - 1][x + 1].get());
    }
    if (d2 && isWalkableAt(x + 1, y + 1))
    {
        neighbors.push_back(nodes_[y + 1][x + 1].get());
    }
    if (d3 && isWalkableAt(x - 1, y + 1))
    {
        neighbors.push_back(nodes_[y + 1][x - 1].get());
    }

    return neighbors;
}

Grid Grid::clone() const
{
    Grid newGrid(width, height);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            newGrid.nodes_[i][j]->walkable = nodes_[i][j]->walkable;
        }
    }
    return newGrid;
}

void Grid::resetNodes()
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            nodes_[i][j]->reset();
        }
    }
}
} // namespace pathfinding
