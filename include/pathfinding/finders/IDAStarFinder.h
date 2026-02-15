#ifndef PATHFINDING_FINDERS_IDA_STAR_FINDER_H
#define PATHFINDING_FINDERS_IDA_STAR_FINDER_H

#include "pathfinding/finders/Finder.h"
#include "pathfinding/core/Heuristic.hpp"

namespace pathfinding
{
class IDAStarFinder : public Finder
{
public:
    IDAStarFinder();
    explicit IDAStarFinder(const FinderOptions& options);

    Util::Path findPath(int startX, int startY, int endX, int endY, Grid& grid) override;

private:
    void initHeuristic();
    double search(
        Node *node, double g, double cutoff, Util::Path& route, int depth, Node *endNode, Grid& grid, double startTime);

    Heuristic::HeuristicFunc heuristic_;
    bool trackRecursion_ = false;
    double timeLimit_ = 0.0;
};
} // namespace pathfinding

#endif // !PATHFINDING_FINDERS_IDA_STAR_FINDER_H
