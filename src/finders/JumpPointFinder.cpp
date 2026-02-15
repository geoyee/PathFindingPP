#include "pathfinding/finders/JumpPointFinder.h"

#include <algorithm>
#include <cmath>

namespace pathfinding
{
JumpPointFinder::JumpPointFinder() : Finder()
{
    initHeuristic();
}

JumpPointFinder::JumpPointFinder(const FinderOptions& options) : Finder(options)
{
    initHeuristic();
}

void JumpPointFinder::initHeuristic()
{
    heuristic_ = Heuristic::manhattan;
    if (options_.diagonalMovement == DiagonalMovement::Never)
    {
        heuristic_ = Heuristic::manhattan;
    }
    else
    {
        heuristic_ = Heuristic::octile;
    }
}

void JumpPointFinder::pushNode(Node *node)
{
    openList_.push_back(node);
    std::push_heap(openList_.begin(), openList_.end(), [](Node *a, Node *b) { return a->f > b->f; });
}

Node *JumpPointFinder::popNode()
{
    std::pop_heap(openList_.begin(), openList_.end(), [](Node *a, Node *b) { return a->f > b->f; });
    Node *node = openList_.back();
    openList_.pop_back();
    return node;
}

void JumpPointFinder::updateNode(Node *node)
{
    for (size_t i = 0; i < openList_.size(); ++i)
    {
        if (openList_[i] == node)
        {
            std::make_heap(openList_.begin(), openList_.end(), [](Node *a, Node *b) { return a->f > b->f; });
            break;
        }
    }
}

Util::Path JumpPointFinder::findPath(int startX, int startY, int endX, int endY, Grid& grid)
{
    grid_ = &grid;
    startNode_ = grid.getNodeAt(startX, startY);
    endNode_ = grid.getNodeAt(endX, endY);
    openList_.clear();

    startNode_->g = 0;
    startNode_->f = 0;

    pushNode(startNode_);
    startNode_->opened = true;

    std::function<std::pair<int, int>(int, int, int, int)> jumpFunc;
    std::function<std::vector<std::pair<int, int>>(Node *)> neighborFunc;

    switch (options_.diagonalMovement)
    {
        case DiagonalMovement::Never :
            jumpFunc = [this](int x, int y, int px, int py) { return this->jumpNeverDiagonal(x, y, px, py); };
            neighborFunc = [this](Node *node) { return this->findNeighborsNeverDiagonal(node); };
            break;
        case DiagonalMovement::Always :
            jumpFunc = [this](int x, int y, int px, int py) { return this->jumpAlwaysDiagonal(x, y, px, py); };
            neighborFunc = [this](Node *node) { return this->findNeighborsAlwaysDiagonal(node); };
            break;
        case DiagonalMovement::OnlyWhenNoObstacles :
            jumpFunc = [this](int x, int y, int px, int py) { return this->jumpIfNoObstacles(x, y, px, py); };
            neighborFunc = [this](Node *node) { return this->findNeighborsIfNoObstacles(node); };
            break;
        case DiagonalMovement::IfAtMostOneObstacle :
        default :
            jumpFunc = [this](int x, int y, int px, int py) { return this->jumpIfAtMostOneObstacle(x, y, px, py); };
            neighborFunc = [this](Node *node) { return this->findNeighborsIfAtMostOneObstacle(node); };
            break;
    }

    while (!openList_.empty())
    {
        Node *node = popNode();
        node->closed = true;

        if (node == endNode_)
        {
            return Util::expandPath(Util::backtrace(endNode_));
        }

        identifySuccessors(node, jumpFunc, neighborFunc);
    }

    return {};
}

void JumpPointFinder::identifySuccessors(Node *node,
                                         std::function<std::pair<int, int>(int, int, int, int)> jumpFunc,
                                         std::function<std::vector<std::pair<int, int>>(Node *)> neighborFunc)
{
    auto neighbors = neighborFunc(node);

    for (auto& neighbor : neighbors)
    {
        auto jumpPoint = jumpFunc(neighbor.first, neighbor.second, node->x, node->y);

        if (jumpPoint.first >= 0 && jumpPoint.second >= 0)
        {
            int jx = jumpPoint.first;
            int jy = jumpPoint.second;

            if (!grid_->isInside(jx, jy))
            {
                continue;
            }

            Node *jumpNode = grid_->getNodeAt(jx, jy);

            if (jumpNode->closed)
            {
                continue;
            }

            double d = Heuristic::octile(std::abs(jx - node->x), std::abs(jy - node->y));
            double ng = node->g + d;

            if (!jumpNode->opened || ng < jumpNode->g)
            {
                jumpNode->g = ng;
                jumpNode->h =
                    jumpNode->h > 0 ? jumpNode->h : heuristic_(std::abs(jx - endNode_->x), std::abs(jy - endNode_->y));
                jumpNode->f = jumpNode->g + jumpNode->h;
                jumpNode->parent = node;

                if (!jumpNode->opened)
                {
                    pushNode(jumpNode);
                    jumpNode->opened = true;
                }
                else
                {
                    updateNode(jumpNode);
                }
            }
        }
    }
}

std::pair<int, int> JumpPointFinder::jumpNeverDiagonal(int x, int y, int px, int py)
{
    int dx = x - px;
    int dy = y - py;

    if (!grid_->isWalkableAt(x, y))
    {
        return {-1, -1};
    }

    if (trackJumpRecursion_)
    {
        grid_->getNodeAt(x, y)->tested = true;
    }

    if (grid_->getNodeAt(x, y) == endNode_)
    {
        return {x, y};
    }

    if (dx != 0)
    {
        if ((grid_->isWalkableAt(x, y - 1) && !grid_->isWalkableAt(x - dx, y - 1)) ||
            (grid_->isWalkableAt(x, y + 1) && !grid_->isWalkableAt(x - dx, y + 1)))
        {
            return {x, y};
        }
    }
    else if (dy != 0)
    {
        if ((grid_->isWalkableAt(x - 1, y) && !grid_->isWalkableAt(x - 1, y - dy)) ||
            (grid_->isWalkableAt(x + 1, y) && !grid_->isWalkableAt(x + 1, y - dy)))
        {
            return {x, y};
        }

        if (jumpNeverDiagonal(x + 1, y, x, y).first >= 0 || jumpNeverDiagonal(x - 1, y, x, y).first >= 0)
        {
            return {x, y};
        }
    }

    return jumpNeverDiagonal(x + dx, y + dy, x, y);
}

std::pair<int, int> JumpPointFinder::jumpAlwaysDiagonal(int x, int y, int px, int py)
{
    int dx = x - px;
    int dy = y - py;

    if (!grid_->isWalkableAt(x, y))
    {
        return {-1, -1};
    }

    if (trackJumpRecursion_)
    {
        grid_->getNodeAt(x, y)->tested = true;
    }

    if (grid_->getNodeAt(x, y) == endNode_)
    {
        return {x, y};
    }

    if (dx != 0 && dy != 0)
    {
        if ((grid_->isWalkableAt(x - dx, y + dy) && !grid_->isWalkableAt(x - dx, y)) ||
            (grid_->isWalkableAt(x + dx, y - dy) && !grid_->isWalkableAt(x, y - dy)))
        {
            return {x, y};
        }

        if (jumpAlwaysDiagonal(x + dx, y, x, y).first >= 0 || jumpAlwaysDiagonal(x, y + dy, x, y).first >= 0)
        {
            return {x, y};
        }
    }
    else
    {
        if (dx != 0)
        {
            if ((grid_->isWalkableAt(x + dx, y + 1) && !grid_->isWalkableAt(x, y + 1)) ||
                (grid_->isWalkableAt(x + dx, y - 1) && !grid_->isWalkableAt(x, y - 1)))
            {
                return {x, y};
            }
        }
        else
        {
            if ((grid_->isWalkableAt(x + 1, y + dy) && !grid_->isWalkableAt(x + 1, y)) ||
                (grid_->isWalkableAt(x - 1, y + dy) && !grid_->isWalkableAt(x - 1, y)))
            {
                return {x, y};
            }
        }
    }

    return jumpAlwaysDiagonal(x + dx, y + dy, x, y);
}

std::pair<int, int> JumpPointFinder::jumpIfNoObstacles(int x, int y, int px, int py)
{
    int dx = x - px;
    int dy = y - py;

    if (!grid_->isWalkableAt(x, y))
    {
        return {-1, -1};
    }

    if (trackJumpRecursion_)
    {
        grid_->getNodeAt(x, y)->tested = true;
    }

    if (grid_->getNodeAt(x, y) == endNode_)
    {
        return {x, y};
    }

    if (dx != 0 && dy != 0)
    {
        if (jumpIfNoObstacles(x + dx, y, x, y).first >= 0 || jumpIfNoObstacles(x, y + dy, x, y).first >= 0)
        {
            return {x, y};
        }
    }
    else
    {
        if (dx != 0)
        {
            if ((grid_->isWalkableAt(x, y - 1) && !grid_->isWalkableAt(x - dx, y - 1)) ||
                (grid_->isWalkableAt(x, y + 1) && !grid_->isWalkableAt(x - dx, y + 1)))
            {
                return {x, y};
            }
        }
        else if (dy != 0)
        {
            if ((grid_->isWalkableAt(x - 1, y) && !grid_->isWalkableAt(x - 1, y - dy)) ||
                (grid_->isWalkableAt(x + 1, y) && !grid_->isWalkableAt(x + 1, y - dy)))
            {
                return {x, y};
            }
        }
    }

    if (grid_->isWalkableAt(x + dx, y) && grid_->isWalkableAt(x, y + dy))
    {
        return jumpIfNoObstacles(x + dx, y + dy, x, y);
    }

    return {-1, -1};
}

std::pair<int, int> JumpPointFinder::jumpIfAtMostOneObstacle(int x, int y, int px, int py)
{
    int dx = x - px;
    int dy = y - py;

    if (!grid_->isWalkableAt(x, y))
    {
        return {-1, -1};
    }

    if (trackJumpRecursion_)
    {
        grid_->getNodeAt(x, y)->tested = true;
    }

    if (grid_->getNodeAt(x, y) == endNode_)
    {
        return {x, y};
    }

    if (dx != 0 && dy != 0)
    {
        if ((grid_->isWalkableAt(x - dx, y + dy) && !grid_->isWalkableAt(x - dx, y)) ||
            (grid_->isWalkableAt(x + dx, y - dy) && !grid_->isWalkableAt(x, y - dy)))
        {
            return {x, y};
        }

        if (jumpIfAtMostOneObstacle(x + dx, y, x, y).first >= 0 || jumpIfAtMostOneObstacle(x, y + dy, x, y).first >= 0)
        {
            return {x, y};
        }
    }
    else
    {
        if (dx != 0)
        {
            if ((grid_->isWalkableAt(x + dx, y + 1) && !grid_->isWalkableAt(x, y + 1)) ||
                (grid_->isWalkableAt(x + dx, y - 1) && !grid_->isWalkableAt(x, y - 1)))
            {
                return {x, y};
            }
        }
        else
        {
            if ((grid_->isWalkableAt(x + 1, y + dy) && !grid_->isWalkableAt(x + 1, y)) ||
                (grid_->isWalkableAt(x - 1, y + dy) && !grid_->isWalkableAt(x - 1, y)))
            {
                return {x, y};
            }
        }
    }

    if (grid_->isWalkableAt(x + dx, y) || grid_->isWalkableAt(x, y + dy))
    {
        return jumpIfAtMostOneObstacle(x + dx, y + dy, x, y);
    }

    return {-1, -1};
}

std::vector<std::pair<int, int>> JumpPointFinder::findNeighborsNeverDiagonal(Node *node)
{
    std::vector<std::pair<int, int>> neighbors;
    Node *parent = node->parent;
    int x = node->x;
    int y = node->y;

    if (parent)
    {
        int px = parent->x;
        int py = parent->y;
        int dx = (x - px) / std::max(std::abs(x - px), 1);
        int dy = (y - py) / std::max(std::abs(y - py), 1);

        if (dx != 0)
        {
            if (grid_->isWalkableAt(x, y - 1))
            {
                neighbors.push_back({x, y - 1});
            }
            if (grid_->isWalkableAt(x, y + 1))
            {
                neighbors.push_back({x, y + 1});
            }
            if (grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y});
            }
        }
        else if (dy != 0)
        {
            if (grid_->isWalkableAt(x - 1, y))
            {
                neighbors.push_back({x - 1, y});
            }
            if (grid_->isWalkableAt(x + 1, y))
            {
                neighbors.push_back({x + 1, y});
            }
            if (grid_->isWalkableAt(x, y + dy))
            {
                neighbors.push_back({x, y + dy});
            }
        }
    }
    else
    {
        auto nodes = grid_->getNeighbors(node, DiagonalMovement::Never);
        for (Node *n : nodes)
        {
            neighbors.push_back({n->x, n->y});
        }
    }

    return neighbors;
}

std::vector<std::pair<int, int>> JumpPointFinder::findNeighborsAlwaysDiagonal(Node *node)
{
    std::vector<std::pair<int, int>> neighbors;
    Node *parent = node->parent;
    int x = node->x;
    int y = node->y;

    if (parent)
    {
        int px = parent->x;
        int py = parent->y;
        int dx = (x - px) / std::max(std::abs(x - px), 1);
        int dy = (y - py) / std::max(std::abs(y - py), 1);

        if (dx != 0 && dy != 0)
        {
            if (grid_->isWalkableAt(x, y + dy))
            {
                neighbors.push_back({x, y + dy});
            }
            if (grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y});
            }
            if (grid_->isWalkableAt(x + dx, y + dy))
            {
                neighbors.push_back({x + dx, y + dy});
            }
            if (!grid_->isWalkableAt(x - dx, y))
            {
                neighbors.push_back({x - dx, y + dy});
            }
            if (!grid_->isWalkableAt(x, y - dy))
            {
                neighbors.push_back({x + dx, y - dy});
            }
        }
        else
        {
            if (dx == 0)
            {
                if (grid_->isWalkableAt(x, y + dy))
                {
                    neighbors.push_back({x, y + dy});
                }
                if (!grid_->isWalkableAt(x + 1, y))
                {
                    neighbors.push_back({x + 1, y + dy});
                }
                if (!grid_->isWalkableAt(x - 1, y))
                {
                    neighbors.push_back({x - 1, y + dy});
                }
            }
            else
            {
                if (grid_->isWalkableAt(x + dx, y))
                {
                    neighbors.push_back({x + dx, y});
                }
                if (!grid_->isWalkableAt(x, y + 1))
                {
                    neighbors.push_back({x + dx, y + 1});
                }
                if (!grid_->isWalkableAt(x, y - 1))
                {
                    neighbors.push_back({x + dx, y - 1});
                }
            }
        }
    }
    else
    {
        auto nodes = grid_->getNeighbors(node, DiagonalMovement::Always);
        for (Node *n : nodes)
        {
            neighbors.push_back({n->x, n->y});
        }
    }

    return neighbors;
}

std::vector<std::pair<int, int>> JumpPointFinder::findNeighborsIfNoObstacles(Node *node)
{
    std::vector<std::pair<int, int>> neighbors;
    Node *parent = node->parent;
    int x = node->x;
    int y = node->y;

    if (parent)
    {
        int px = parent->x;
        int py = parent->y;
        int dx = (x - px) / std::max(std::abs(x - px), 1);
        int dy = (y - py) / std::max(std::abs(y - py), 1);

        if (dx != 0 && dy != 0)
        {
            if (grid_->isWalkableAt(x, y + dy))
            {
                neighbors.push_back({x, y + dy});
            }
            if (grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y});
            }
            if (grid_->isWalkableAt(x, y + dy) && grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y + dy});
            }
        }
        else
        {
            if (dx != 0)
            {
                bool isNextWalkable = grid_->isWalkableAt(x + dx, y);
                bool isTopWalkable = grid_->isWalkableAt(x, y + 1);
                bool isBottomWalkable = grid_->isWalkableAt(x, y - 1);

                if (isNextWalkable)
                {
                    neighbors.push_back({x + dx, y});
                    if (isTopWalkable)
                    {
                        neighbors.push_back({x + dx, y + 1});
                    }
                    if (isBottomWalkable)
                    {
                        neighbors.push_back({x + dx, y - 1});
                    }
                }
                if (isTopWalkable)
                {
                    neighbors.push_back({x, y + 1});
                }
                if (isBottomWalkable)
                {
                    neighbors.push_back({x, y - 1});
                }
            }
            else if (dy != 0)
            {
                bool isNextWalkable = grid_->isWalkableAt(x, y + dy);
                bool isRightWalkable = grid_->isWalkableAt(x + 1, y);
                bool isLeftWalkable = grid_->isWalkableAt(x - 1, y);

                if (isNextWalkable)
                {
                    neighbors.push_back({x, y + dy});
                    if (isRightWalkable)
                    {
                        neighbors.push_back({x + 1, y + dy});
                    }
                    if (isLeftWalkable)
                    {
                        neighbors.push_back({x - 1, y + dy});
                    }
                }
                if (isRightWalkable)
                {
                    neighbors.push_back({x + 1, y});
                }
                if (isLeftWalkable)
                {
                    neighbors.push_back({x - 1, y});
                }
            }
        }
    }
    else
    {
        auto nodes = grid_->getNeighbors(node, DiagonalMovement::OnlyWhenNoObstacles);
        for (Node *n : nodes)
        {
            neighbors.push_back({n->x, n->y});
        }
    }

    return neighbors;
}

std::vector<std::pair<int, int>> JumpPointFinder::findNeighborsIfAtMostOneObstacle(Node *node)
{
    std::vector<std::pair<int, int>> neighbors;
    Node *parent = node->parent;
    int x = node->x;
    int y = node->y;

    if (parent)
    {
        int px = parent->x;
        int py = parent->y;
        int dx = (x - px) / std::max(std::abs(x - px), 1);
        int dy = (y - py) / std::max(std::abs(y - py), 1);

        if (dx != 0 && dy != 0)
        {
            if (grid_->isWalkableAt(x, y + dy))
            {
                neighbors.push_back({x, y + dy});
            }
            if (grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y});
            }
            if (grid_->isWalkableAt(x, y + dy) || grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y + dy});
            }
            if (!grid_->isWalkableAt(x - dx, y) && grid_->isWalkableAt(x, y + dy))
            {
                neighbors.push_back({x - dx, y + dy});
            }
            if (!grid_->isWalkableAt(x, y - dy) && grid_->isWalkableAt(x + dx, y))
            {
                neighbors.push_back({x + dx, y - dy});
            }
        }
        else
        {
            if (dx == 0)
            {
                if (grid_->isWalkableAt(x, y + dy))
                {
                    neighbors.push_back({x, y + dy});
                    if (!grid_->isWalkableAt(x + 1, y))
                    {
                        neighbors.push_back({x + 1, y + dy});
                    }
                    if (!grid_->isWalkableAt(x - 1, y))
                    {
                        neighbors.push_back({x - 1, y + dy});
                    }
                }
            }
            else
            {
                if (grid_->isWalkableAt(x + dx, y))
                {
                    neighbors.push_back({x + dx, y});
                    if (!grid_->isWalkableAt(x, y + 1))
                    {
                        neighbors.push_back({x + dx, y + 1});
                    }
                    if (!grid_->isWalkableAt(x, y - 1))
                    {
                        neighbors.push_back({x + dx, y - 1});
                    }
                }
            }
        }
    }
    else
    {
        auto nodes = grid_->getNeighbors(node, DiagonalMovement::IfAtMostOneObstacle);
        for (Node *n : nodes)
        {
            neighbors.push_back({n->x, n->y});
        }
    }

    return neighbors;
}
} // namespace pathfinding
