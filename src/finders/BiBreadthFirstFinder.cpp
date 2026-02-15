#include "pathfinding/finders/BiBreadthFirstFinder.h"

namespace pathfinding
{
BiBreadthFirstFinder::BiBreadthFirstFinder() : Finder() { }

BiBreadthFirstFinder::BiBreadthFirstFinder(const FinderOptions& options) : Finder(options) { }

util::Path BiBreadthFirstFinder::findPath(int startX, int startY, int endX, int endY, Grid& grid)
{
    Node *startNode = grid.getNodeAt(startX, startY);
    Node *endNode = grid.getNodeAt(endX, endY);

    std::queue<Node *> startOpenList;
    std::queue<Node *> endOpenList;

    startOpenList.push(startNode);
    startNode->opened = true;
    startNode->openedBy = BY_START;

    endOpenList.push(endNode);
    endNode->opened = true;
    endNode->openedBy = BY_END;

    while (!startOpenList.empty() && !endOpenList.empty())
    {
        Node *node = startOpenList.front();
        startOpenList.pop();
        node->closed = true;

        auto neighbors = grid.getNeighbors(node, options_.diagonalMovement);
        for (Node *neighbor : neighbors)
        {
            if (neighbor->closed)
            {
                continue;
            }
            if (neighbor->opened)
            {
                if (neighbor->openedBy == BY_END)
                {
                    return util::biBacktrace(node, neighbor);
                }
                continue;
            }
            startOpenList.push(neighbor);
            neighbor->parent = node;
            neighbor->opened = true;
            neighbor->openedBy = BY_START;
        }

        node = endOpenList.front();
        endOpenList.pop();
        node->closed = true;

        neighbors = grid.getNeighbors(node, options_.diagonalMovement);
        for (Node *neighbor : neighbors)
        {
            if (neighbor->closed)
            {
                continue;
            }
            if (neighbor->opened)
            {
                if (neighbor->openedBy == BY_START)
                {
                    return util::biBacktrace(neighbor, node);
                }
                continue;
            }
            endOpenList.push(neighbor);
            neighbor->parent = node;
            neighbor->opened = true;
            neighbor->openedBy = BY_END;
        }
    }

    return {};
}
} // namespace pathfinding
