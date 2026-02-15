#ifndef PATHFINDING_CORE_NODE_H
#define PATHFINDING_CORE_NODE_H

#include "pathfinding/core/Export.hpp"

namespace pathfinding
{
class PATHFINDING_API Node
{
public:
    int x;
    int y;
    bool walkable;
    double g = 0.0;
    double h = 0.0;
    double f = 0.0;

    Node(int x, int y, bool walkable = true);

    Node *parent = nullptr;
    bool opened = false;
    bool closed = false;
    int openedBy = 0;
    bool tested = false;
    int retainCount = 0;
    void reset();
};
} // namespace pathfinding

#endif // !PATHFINDING_CORE_NODE_H
