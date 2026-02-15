#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "pathfinding/PathFinding.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>

#include <vector>

using namespace pathfinding;

namespace
{
std::vector<std::vector<int>> getTestMatrix()
{
    return {
        {1, 0, 0, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {1, 0, 0, 1},
    };
}

struct TestScenario
{
    int startX, startY, endX, endY;
    std::vector<std::vector<int>> matrix;
    size_t expectedLength;
};

std::vector<TestScenario> getScenarios()
{
    return {
        {0, 0, 1, 1, {{0, 0}, {1, 0}}, 3},
        {1,
         1, 4,
         4, {{0, 0, 0, 0, 0}, {1, 0, 1, 1, 0}, {1, 0, 1, 0, 0}, {0, 1, 0, 0, 0}, {1, 0, 1, 1, 0}, {0, 0, 1, 0, 0}},
         9},
        {0,
         3, 3,
         3, {{0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 0, 1, 1, 0}, {0, 0, 0, 0, 0}},
         10},
        {4, 4, 19, 19, std::vector<std::vector<int>>(20, std::vector<int>(20, 0)), 31},
    };
}
} // namespace

TEST_CASE("Grid - generate without matrix", "[grid]")
{
    int width = 10;
    int height = 20;
    Grid grid(width, height);

    SECTION("should have correct size")
    {
        REQUIRE(grid.width == width);
        REQUIRE(grid.height == height);
    }

    SECTION("should set all nodes walkable")
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                REQUIRE(grid.isWalkableAt(x, y) == true);
            }
        }
    }
}

TEST_CASE("Grid - generate with matrix", "[grid]")
{
    auto matrix = getTestMatrix();
    int height = static_cast<int>(matrix.size());
    int width = static_cast<int>(matrix[0].size());
    Grid grid(matrix);

    SECTION("should have correct size")
    {
        REQUIRE(grid.width == width);
        REQUIRE(grid.height == height);
    }

    SECTION("should initiate all nodes walkable attribute")
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (matrix[y][x])
                {
                    REQUIRE(grid.isWalkableAt(x, y) == false);
                }
                else
                {
                    REQUIRE(grid.isWalkableAt(x, y) == true);
                }
            }
        }
    }

    SECTION("should be able to set nodes walkable attribute")
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                grid.setWalkableAt(x, y, false);
            }
        }
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                REQUIRE(grid.isWalkableAt(x, y) == false);
            }
        }
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                grid.setWalkableAt(x, y, true);
            }
        }
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                REQUIRE(grid.isWalkableAt(x, y) == true);
            }
        }
    }

    SECTION("should return correct answer for position validity query")
    {
        REQUIRE(grid.isInside(0, 0) == true);
        REQUIRE(grid.isInside(0, height - 1) == true);
        REQUIRE(grid.isInside(width - 1, 0) == true);
        REQUIRE(grid.isInside(width - 1, height - 1) == true);
        REQUIRE(grid.isInside(-1, -1) == false);
        REQUIRE(grid.isInside(0, -1) == false);
        REQUIRE(grid.isInside(-1, 0) == false);
        REQUIRE(grid.isInside(0, height) == false);
        REQUIRE(grid.isInside(width, 0) == false);
        REQUIRE(grid.isInside(width, height) == false);
    }

    SECTION("should return correct neighbors")
    {
        auto neighbors = grid.getNeighbors(grid.getNodeAt(0, 1), DiagonalMovement::Never);
        REQUIRE(neighbors.size() == 1);
        REQUIRE(neighbors[0] == grid.getNodeAt(0, 2));
    }
}

TEST_CASE("Grid - clone", "[grid]")
{
    auto matrix = getTestMatrix();
    Grid grid(matrix);
    Grid cloned = grid.clone();

    SECTION("cloned grid should have same walkable status")
    {
        for (int y = 0; y < grid.height; ++y)
        {
            for (int x = 0; x < grid.width; ++x)
            {
                REQUIRE(cloned.isWalkableAt(x, y) == grid.isWalkableAt(x, y));
            }
        }
    }

    SECTION("modifying cloned grid should not affect original")
    {
        cloned.setWalkableAt(0, 0, true);
        REQUIRE(grid.isWalkableAt(0, 0) == false);
    }
}

TEST_CASE("Heuristic", "[heuristic]")
{
    SECTION("manhattan")
    {
        REQUIRE(heuristic::manhattan(3, 4) == 7);
        REQUIRE(heuristic::manhattan(0, 0) == 0);
    }

    SECTION("euclidean")
    {
        REQUIRE(heuristic::euclidean(3, 4) == Catch::Approx(5.0));
        REQUIRE(heuristic::euclidean(0, 0) == Catch::Approx(0.0));
    }

    SECTION("chebyshev")
    {
        REQUIRE(heuristic::chebyshev(3, 4) == 4);
        REQUIRE(heuristic::chebyshev(4, 3) == 4);
    }

    SECTION("octile")
    {
        double result = heuristic::octile(3, 4);
        REQUIRE(result >= 4.0);
        REQUIRE(result <= 6.0);
        REQUIRE(result == Catch::Approx(4.0 + (std::sqrt(2.0) - 1.0) * 3.0));
    }
}

TEST_CASE("Util - interpolate", "[util]")
{
    SECTION("should return the interpolated path")
    {
        auto path = util::interpolate(0, 1, 0, 4);
        REQUIRE(path.size() == 4);
        REQUIRE(path[0] == std::make_pair(0, 1));
        REQUIRE(path[1] == std::make_pair(0, 2));
        REQUIRE(path[2] == std::make_pair(0, 3));
        REQUIRE(path[3] == std::make_pair(0, 4));
    }
}

TEST_CASE("Util - expandPath", "[util]")
{
    SECTION("should return empty array given empty array")
    {
        auto result = util::expandPath({});
        REQUIRE(result.empty());
    }

    SECTION("should return the expanded path")
    {
        util::Path path = {
            {0, 1},
            {0, 4}
        };
        auto result = util::expandPath(path);
        REQUIRE(result.size() == 4);
        REQUIRE(result[0] == std::make_pair(0, 1));
        REQUIRE(result[3] == std::make_pair(0, 4));
    }

    SECTION("should expand multi-segment path")
    {
        util::Path path = {
            {0, 1},
            {0, 4},
            {2, 6}
        };
        auto result = util::expandPath(path);
        REQUIRE(result.size() == 6);
    }
}

TEST_CASE("Util - compressPath", "[util]")
{
    SECTION("should return original path if too short")
    {
        util::Path path = {
            {0, 1}
        };
        auto result = util::compressPath(path);
        REQUIRE(result.size() == 1);
    }

    SECTION("should return compressed path")
    {
        util::Path path = {
            {0, 1},
            {0, 2},
            {0, 3},
            {0, 4}
        };
        auto result = util::compressPath(path);
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == std::make_pair(0, 1));
        REQUIRE(result[1] == std::make_pair(0, 4));
    }

    SECTION("should compress multi-segment path")
    {
        util::Path path = {
            {0, 1},
            {0, 2},
            {0, 3},
            {0, 4},
            {1, 5},
            {2, 6}
        };
        auto result = util::compressPath(path);
        REQUIRE(result.size() == 3);
    }
}

template<typename FinderType>
void testFinderOptimal(const std::string& name)
{
    auto scenarios = getScenarios();
    int testId = 0;

    for (const auto& scen : scenarios)
    {
        SECTION(name + " - maze " + std::to_string(++testId))
        {
            Grid grid(scen.matrix);
            FinderType finder;
            auto path = finder.findPath(scen.startX, scen.startY, scen.endX, scen.endY, grid);

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() == scen.expectedLength);
            REQUIRE(path.front().first == scen.startX);
            REQUIRE(path.front().second == scen.startY);
            REQUIRE(path.back().first == scen.endX);
            REQUIRE(path.back().second == scen.endY);
        }
    }
}

template<typename FinderType>
void testFinderNonOptimal(const std::string& name)
{
    auto scenarios = getScenarios();
    int testId = 0;

    for (const auto& scen : scenarios)
    {
        SECTION(name + " - maze " + std::to_string(++testId))
        {
            Grid grid(scen.matrix);
            FinderType finder;
            auto path = finder.findPath(scen.startX, scen.startY, scen.endX, scen.endY, grid);

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.front().first == scen.startX);
            REQUIRE(path.front().second == scen.startY);
            REQUIRE(path.back().first == scen.endX);
            REQUIRE(path.back().second == scen.endY);
        }
    }
}

TEST_CASE("AStarFinder - optimal path", "[finders]")
{
    testFinderOptimal<AStarFinder>("AStar");
}

TEST_CASE("BreadthFirstFinder - optimal path", "[finders]")
{
    testFinderOptimal<BreadthFirstFinder>("BreadthFirst");
}

TEST_CASE("DijkstraFinder - optimal path", "[finders]")
{
    testFinderOptimal<DijkstraFinder>("Dijkstra");
}

TEST_CASE("BiBreadthFirstFinder - optimal path", "[finders]")
{
    testFinderOptimal<BiBreadthFirstFinder>("BiBreadthFirst");
}

TEST_CASE("BiDijkstraFinder - optimal path", "[finders]")
{
    testFinderOptimal<BiDijkstraFinder>("BiDijkstra");
}

TEST_CASE("BiAStarFinder - non-optimal path", "[finders]")
{
    testFinderNonOptimal<BiAStarFinder>("BiAStar");
}

TEST_CASE("BestFirstFinder - non-optimal path", "[finders]")
{
    testFinderNonOptimal<BestFirstFinder>("BestFirst");
}

TEST_CASE("BiBestFirstFinder - non-optimal path", "[finders]")
{
    testFinderNonOptimal<BiBestFirstFinder>("BiBestFirst");
}

TEST_CASE("IDAStarFinder - non-optimal path", "[finders]")
{
    testFinderNonOptimal<IDAStarFinder>("IDAStar");
}

TEST_CASE("JumpPointFinder - non-optimal path", "[finders]")
{
    auto scenarios = getScenarios();
    int testId = 0;

    for (const auto& scen : scenarios)
    {
        SECTION("JPFMoveDiagonallyIfAtMostOneObstacle - maze " + std::to_string(++testId))
        {
            Grid grid(scen.matrix);
            FinderOptions opts;
            opts.diagonalMovement = DiagonalMovement::IfAtMostOneObstacle;
            JumpPointFinder finder(opts);
            auto path = finder.findPath(scen.startX, scen.startY, scen.endX, scen.endY, grid);

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.front().first == scen.startX);
            REQUIRE(path.front().second == scen.startY);
            REQUIRE(path.back().first == scen.endX);
            REQUIRE(path.back().second == scen.endY);
        }
    }

    testId = 0;
    for (const auto& scen : scenarios)
    {
        SECTION("JPFNeverMoveDiagonally - maze " + std::to_string(++testId))
        {
            Grid grid(scen.matrix);
            FinderOptions opts;
            opts.diagonalMovement = DiagonalMovement::Never;
            JumpPointFinder finder(opts);
            auto path = finder.findPath(scen.startX, scen.startY, scen.endX, scen.endY, grid);

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.front().first == scen.startX);
            REQUIRE(path.front().second == scen.startY);
            REQUIRE(path.back().first == scen.endX);
            REQUIRE(path.back().second == scen.endY);
        }
    }
}

TEST_CASE("Finder with diagonal movement", "[finders]")
{
    std::vector<std::vector<int>> matrix = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    };

    SECTION("AStar with diagonal")
    {
        Grid grid(matrix);
        FinderOptions opts;
        opts.allowDiagonal = true;
        AStarFinder finder(opts);
        auto path = finder.findPath(0, 0, 2, 2, grid);

        REQUIRE_FALSE(path.empty());
        REQUIRE(path.size() <= 3);
    }
}

TEST_CASE("Benchmarks", "[benchmark]")
{
    auto scenarios = getScenarios();
    auto largeMatrix = std::vector<std::vector<int>>(50, std::vector<int>(50, 0));

    BENCHMARK("AStarFinder - small maze")
    {
        Grid grid(scenarios[1].matrix);
        AStarFinder finder;
        return finder.findPath(scenarios[1].startX, scenarios[1].startY, scenarios[1].endX, scenarios[1].endY, grid);
    };

    BENCHMARK("AStarFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        AStarFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("BreadthFirstFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        BreadthFirstFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("DijkstraFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        DijkstraFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("BiAStarFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        BiAStarFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("BiBreadthFirstFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        BiBreadthFirstFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("BiDijkstraFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        BiDijkstraFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("IDAStarFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        IDAStarFinder finder;
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("JumpPointFinder - 50x50 empty grid")
    {
        Grid grid(largeMatrix);
        FinderOptions opts;
        opts.diagonalMovement = DiagonalMovement::IfAtMostOneObstacle;
        JumpPointFinder finder(opts);
        return finder.findPath(0, 0, 49, 49, grid);
    };

    BENCHMARK("Grid clone - 50x50")
    {
        Grid grid(largeMatrix);
        return grid.clone();
    };

    BENCHMARK("Path compression")
    {
        util::Path path;
        for (int i = 0; i < 100; ++i)
        {
            path.push_back({i, 0});
        }
        return util::compressPath(path);
    };
}
