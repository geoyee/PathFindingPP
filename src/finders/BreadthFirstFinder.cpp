#include "pathfinding/finders/BreadthFirstFinder.h"

namespace pathfinding
{
BreadthFirstFinder::BreadthFirstFinder() : Finder() { }

BreadthFirstFinder::BreadthFirstFinder(const FinderOptions& options) : Finder(options) { }

Util::Path BreadthFirstFinder::findPath(int startX, int startY, int endX, int endY, Grid& grid)
{
    std::queue<Node *> openList;
    Node *startNode = grid.getNodeAt(startX, startY);
    Node *endNode = grid.getNodeAt(endX, endY);

    openList.push(startNode);
    startNode->opened = true;

    while (!openList.empty())
    {
        Node *node = openList.front();
        openList.pop();
        node->closed = true;

        if (node == endNode)
        {
            return Util::backtrace(endNode);
        }

        auto neighbors = grid.getNeighbors(node, options_.diagonalMovement);
        for (Node *neighbor : neighbors)
        {
            if (neighbor->closed || neighbor->opened)
            {
                continue;
            }

            openList.push(neighbor);
            neighbor->opened = true;
            neighbor->parent = node;
        }
    }

    return {};
}
} // namespace pathfinding
