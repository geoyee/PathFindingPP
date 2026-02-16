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

## 构建及测试

### CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
./PathFinding_test
```

### XMake

```bash
xmake f --test=y
xmake
xmake run PathFinding_test
```

## GUI 可视化工具

### 构建 GUI

```bash
# CMake
mkdir build && cd build
cmake .. -DPATH_FINDING_BUILD_VISUAL=ON
cmake --build .
./PathFinding_visual

# XMake
xmake f --visual=y
xmake
xmake run PathFinding_gui
```

### 使用方法

1. 选择编辑模式（墙壁/起点/终点）
2. 在网格上点击或拖拽绘制
3. 选择算法和参数
4. 点击 "Find Path" 开始寻路

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
- CMake 或 XMake
- Catch2 (仅测试需要)
- Qt6 (仅 GUI 需要)
