# PathFindingPP

[PathFinding.js](https://github.com/qiao/PathFinding.js) 的 C++ 实现

## 支持的算法

| 算法 | 说明 |
|------|------|
| `AStarFinder` | A* 寻路算法 |
| `BreadthFirstFinder` | 广度优先搜索 |
| `DijkstraFinder` | Dijkstra 算法 |
| `BestFirstFinder` | 最佳优先搜索 |
| `BiAStarFinder` | 双向 A* 算法 |
| `BiBreadthFirstFinder` | 双向广度优先搜索 |
| `BiDijkstraFinder` | 双向 Dijkstra 算法 |
| `BiBestFirstFinder` | 双向最佳优先搜索 |
| `IDAStarFinder` | 迭代加深 A* 算法 |
| `JumpPointFinder` | 跳点搜索算法 |

## 构建

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## 运行测试

```bash
cd build
./PathFinding_test
```

## 使用示例

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

## 依赖

- C++14 或更高版本
- CMake 3.15+
- Catch2 (仅测试需要)
