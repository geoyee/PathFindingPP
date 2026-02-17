# PathFindingPP

English | [中文](README_CN.md)

A C++ implementation of [PathFinding.js](https://github.com/qiao/PathFinding.js), along with a Qt6-based visualization tool inspired by the original repository's visualization tool

## Supported Algorithms

| Algorithm | Description |
|-----------|-------------|
| `AStarFinder` | A* pathfinding algorithm |
| `BreadthFirstFinder` | Breadth-First Search |
| `DijkstraFinder` | Dijkstra's algorithm |
| `BestFirstFinder` | Best-First Search |
| `BiAStarFinder` | Bidirectional A* algorithm |
| `BiBreadthFirstFinder` | Bidirectional Breadth-First Search |
| `BiDijkstraFinder` | Bidirectional Dijkstra's algorithm |
| `BiBestFirstFinder` | Bidirectional Best-First Search |
| `IDAStarFinder` | Iterative Deepening A* algorithm |
| `JumpPointFinder` | Jump Point Search algorithm |

## Build and Test

```bash
# CMake
mkdir build && cd build
cmake ..
cmake --build .
./PathFinding_test

# XMake
xmake f --test=y
xmake
xmake run PathFinding_test
```

## Visualization Tool

### Build

```bash
# CMake
mkdir build && cd build
cmake .. -DPATH_FINDING_BUILD_VISUAL=ON
cmake --build .
./PathFinding_visual

# XMake
xmake f --visual=y
xmake
xmake run PathFinding_visual
```

### Usage

1. Select edit mode (Wall/Start/End)
2. Click or drag on the grid to draw
3. Select algorithm and parameters
4. Click "Find Path" to start pathfinding

### Algorithm Options

| Algorithm | Heuristic | Allow Diagonal | Bi-directional | Don't Cross Corners | Weight | Time Limit |
|-----------|-----------|----------------|----------------|---------------------|--------|------------|
| A* | ✓ | ✓ | ✓ | ✓ | ✓ | |
| IDA* | ✓ | ✓ | | ✓ | ✓ | ✓ |
| Breadth First Search | | ✓ | ✓ | ✓ | | |
| Best First Search | ✓ | ✓ | ✓ | ✓ | | |
| Dijkstra | | ✓ | ✓ | ✓ | | |
| Jump Point Search | ✓ | | | | | |
| Orthogonal Jump Point Search | ✓ | | | | | |
| Trace | ✓ | ✓ | | ✓ | | |

## Usage Example

```cpp
#include "pathfinding/PathFinding.h"

int main() {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    
    pathfinding::Grid grid(matrix);
    pathfinding::AStarFinder finder;
    
    auto path = finder.findPath(0, 0, 2, 2, grid);
    
    for (const auto& p : path) {
        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    
    return 0;
}
```

## Dependencies

- C++17 or higher
- CMake or XMake
- Catch2 (for testing only)
- Qt6 (for GUI only)

## License

MIT License
