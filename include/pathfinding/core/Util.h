#ifndef PATHFINDING_CORE_UTIL_H
#define PATHFINDING_CORE_UTIL_H

#include "pathfinding/core/Grid.h"

#include <utility>
#include <vector>

namespace pathfinding
{
namespace Util
{
using Path = std::vector<std::pair<int, int>>;

Path backtrace(const Node *node);
Path biBacktrace(const Node *nodeA, const Node *nodeB);
double pathLength(const Path& path);
Path interpolate(int x0, int y0, int x1, int y1);
Path expandPath(const Path& path);
Path smoothenPath(const Grid& grid, const Path& path);
Path compressPath(const Path& path);
} // namespace Util
} // namespace pathfinding

#endif // !PATHFINDING_CORE_UTIL_H
