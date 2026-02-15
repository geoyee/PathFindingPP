#ifndef PATHFINDING_FINDERS_JUMP_POINT_FINDER_H
#define PATHFINDING_FINDERS_JUMP_POINT_FINDER_H

#include "pathfinding/finders/Finder.h"
#include "pathfinding/core/Heuristic.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace pathfinding
{
class PATHFINDING_API JumpPointFinder : public Finder
{
public:
    JumpPointFinder();
    explicit JumpPointFinder(const FinderOptions& options);

    Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;

private:
    void initHeuristic();
    std::pair<int, int> jumpNeverDiagonal(int x, int y, int px, int py);
    std::pair<int, int> jumpAlwaysDiagonal(int x, int y, int px, int py);
    std::pair<int, int> jumpIfNoObstacles(int x, int y, int px, int py);
    std::pair<int, int> jumpIfAtMostOneObstacle(int x, int y, int px, int py);
    std::vector<std::pair<int, int>> findNeighborsNeverDiagonal(Node *node);
    std::vector<std::pair<int, int>> findNeighborsAlwaysDiagonal(Node *node);
    std::vector<std::pair<int, int>> findNeighborsIfNoObstacles(Node *node);
    std::vector<std::pair<int, int>> findNeighborsIfAtMostOneObstacle(Node *node);
    void identifySuccessors(Node *node,
                            std::function<std::pair<int, int>(int, int, int, int)> jumpFunc,
                            std::function<std::vector<std::pair<int, int>>(Node *)> neighborFunc);
    void pushNode(Node *node);
    Node *popNode();
    void updateNode(Node *node);

    Heuristic::HeuristicFunc heuristic_;
    bool trackJumpRecursion_ = false;
    Grid *grid_ = nullptr;
    Node *startNode_ = nullptr;
    Node *endNode_ = nullptr;
    std::vector<Node *> openList_;
};

} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_JUMP_POINT_FINDER_H
