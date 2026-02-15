#ifndef PATHFINDING_CORE_HEURISTIC_HPP
#define PATHFINDING_CORE_HEURISTIC_HPP

#include <cmath>
#include <functional>

namespace pathfinding
{
namespace Heuristic
{
inline double manhattan(double dx, double dy)
{
    return dx + dy;
}

inline double euclidean(double dx, double dy)
{
    return std::sqrt(dx * dx + dy * dy);
}

inline double octile(double dx, double dy)
{
    static const double F = std::sqrt(2.0) - 1.0;
    return (dx < dy) ? F * dx + dy : F * dy + dx;
}

inline double chebyshev(double dx, double dy)
{
    return std::max(dx, dy);
}

using HeuristicFunc = std::function<double(double, double)>;
} // namespace Heuristic
} // namespace pathfinding

#endif // !PATHFINDING_CORE_HEURISTIC_HPP
