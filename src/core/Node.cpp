#include "pathfinding/core/Node.h"

namespace pathfinding
{
Node::Node(int x, int y, bool walkable) : x(x), y(y), walkable(walkable) { }

void Node::reset()
{
    g = 0.0;
    h = 0.0;
    f = 0.0;
    parent = nullptr;
    opened = false;
    closed = false;
    openedBy = 0;
    tested = false;
    retainCount = 0;
}
} // namespace pathfinding
