#include "pathfinding/core/Util.h"

#include <algorithm>
#include <cmath>

namespace pathfinding
{
namespace Util
{
Path backtrace(const Node *node)
{
    Path path;
    path.push_back({node->x, node->y});

    while (node->parent)
    {
        node = node->parent;
        path.push_back({node->x, node->y});
    }

    std::reverse(path.begin(), path.end());
    return path;
}

Path biBacktrace(const Node *nodeA, const Node *nodeB)
{
    Path pathA = backtrace(nodeA);
    Path pathB = backtrace(nodeB);

    std::reverse(pathB.begin(), pathB.end());
    pathA.insert(pathA.end(), pathB.begin(), pathB.end());

    return pathA;
}

double pathLength(const Path& path)
{
    double sum = 0.0;
    for (size_t i = 1; i < path.size(); ++i)
    {
        int dx = path[i - 1].first - path[i].first;
        int dy = path[i - 1].second - path[i].second;
        sum += std::sqrt(dx * dx + dy * dy);
    }
    return sum;
}

Path interpolate(int x0, int y0, int x1, int y1)
{
    Path line;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        line.push_back({x0, y0});

        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }

    return line;
}

Path expandPath(const Path& path)
{
    Path expanded;

    if (path.size() < 2)
    {
        return expanded;
    }

    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        auto interpolated = interpolate(path[i].first, path[i].second, path[i + 1].first, path[i + 1].second);

        for (size_t j = 0; j < interpolated.size() - 1; ++j)
        {
            expanded.push_back(interpolated[j]);
        }
    }
    expanded.push_back(path.back());

    return expanded;
}

Path smoothenPath(const Grid& grid, const Path& path)
{
    if (path.size() < 2)
    {
        return path;
    }

    int len = static_cast<int>(path.size());
    int x0 = path[0].first;
    int y0 = path[0].second;
    int x1 = path[len - 1].first;
    int y1 = path[len - 1].second;

    int sx = x0, sy = y0;
    Path newPath;
    newPath.push_back({sx, sy});

    for (int i = 2; i < len; ++i)
    {
        int ex = path[i].first;
        int ey = path[i].second;
        auto line = interpolate(sx, sy, ex, ey);

        bool blocked = false;
        for (size_t j = 1; j < line.size(); ++j)
        {
            if (!grid.isWalkableAt(line[j].first, line[j].second))
            {
                blocked = true;
                break;
            }
        }

        if (blocked)
        {
            auto lastValidCoord = path[i - 1];
            newPath.push_back(lastValidCoord);
            sx = lastValidCoord.first;
            sy = lastValidCoord.second;
        }
    }
    newPath.push_back({x1, y1});

    return newPath;
}

Path compressPath(const Path& path)
{
    if (path.size() < 3)
    {
        return path;
    }

    Path compressed;

    int sx = path[0].first;
    int sy = path[0].second;
    int px = path[1].first;
    int py = path[1].second;

    double dx = px - sx;
    double dy = py - sy;
    double sq = std::sqrt(dx * dx + dy * dy);
    dx /= sq;
    dy /= sq;

    compressed.push_back({sx, sy});

    for (size_t i = 2; i < path.size(); ++i)
    {
        int lx = px;
        int ly = py;
        double ldx = dx;
        double ldy = dy;

        px = path[i].first;
        py = path[i].second;

        dx = px - lx;
        dy = py - ly;
        sq = std::sqrt(dx * dx + dy * dy);
        dx /= sq;
        dy /= sq;

        if (dx != ldx || dy != ldy)
        {
            compressed.push_back({lx, ly});
        }
    }

    compressed.push_back({px, py});
    return compressed;
}
} // namespace Util
} // namespace pathfinding
