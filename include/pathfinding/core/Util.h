#ifndef PATHFINDING_CORE_UTIL_H
#define PATHFINDING_CORE_UTIL_H

#include "pathfinding/core/Export.hpp"
#include "pathfinding/core/Grid.h"

#include <utility>
#include <vector>

namespace pathfinding
{
namespace Util
{
using Path = std::vector<std::pair<int, int>>;

PATHFINDING_API Path backtrace(const Node *node);
PATHFINDING_API Path biBacktrace(const Node *nodeA, const Node *nodeB);
PATHFINDING_API double pathLength(const Path& path);
PATHFINDING_API Path interpolate(int x0, int y0, int x1, int y1);
PATHFINDING_API Path expandPath(const Path& path);
PATHFINDING_API Path smoothenPath(const Grid& grid, const Path& path);
PATHFINDING_API Path compressPath(const Path& path);
} // namespace Util
} // namespace pathfinding

#endif // !PATHFINDING_CORE_UTIL_H
