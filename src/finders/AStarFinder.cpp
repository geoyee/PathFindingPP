#include "pathfinding/finders/AStarFinder.h"

#include <algorithm>
#include <cmath>

namespace pathfinding
{
namespace
{
constexpr double SQRT2 = 1.4142135623730951;

template<typename T, typename Compare>
class BinaryHeap
{
public:
    BinaryHeap(Compare cmp) : cmp_(cmp) { }

    void push(T item)
    {
        heap_.push_back(item);
        siftUp(static_cast<int>(heap_.size()) - 1);
    }

    T pop()
    {
        T result = heap_[0];
        T last = heap_.back();
        heap_.pop_back();

        if (!heap_.empty())
        {
            heap_[0] = last;
            siftDown(0);
        }

        return result;
    }

    void updateItem(T item)
    {
        int pos = -1;
        for (int i = 0; i < static_cast<int>(heap_.size()); ++i)
        {
            if (heap_[i] == item)
            {
                pos = i;
                break;
            }
        }
        if (pos >= 0)
        {
            siftUp(pos);
        }
    }

    bool empty() const
    {
        return heap_.empty();
    }

private:
    std::vector<T> heap_;
    Compare cmp_;

    void siftUp(int pos)
    {
        while (pos > 0)
        {
            int parent = (pos - 1) / 2;
            if (cmp_(heap_[pos], heap_[parent]) < 0)
            {
                std::swap(heap_[pos], heap_[parent]);
                pos = parent;
            }
            else
            {
                break;
            }
        }
    }

    void siftDown(int pos)
    {
        int size = static_cast<int>(heap_.size());
        while (true)
        {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            if (left < size && cmp_(heap_[left], heap_[smallest]) < 0)
            {
                smallest = left;
            }
            if (right < size && cmp_(heap_[right], heap_[smallest]) < 0)
            {
                smallest = right;
            }

            if (smallest != pos)
            {
                std::swap(heap_[pos], heap_[smallest]);
                pos = smallest;
            }
            else
            {
                break;
            }
        }
    }
};
} // namespace

AStarFinder::AStarFinder() : Finder()
{
    initHeuristic();
}

AStarFinder::AStarFinder(const FinderOptions& options) : Finder(options)
{
    initHeuristic();
}

void AStarFinder::initHeuristic()
{
    if (options_.diagonalMovement == DiagonalMovement::Never)
    {
        heuristic_ = heuristic::manhattan;
    }
    else
    {
        heuristic_ = heuristic::octile;
    }
}

util::Path AStarFinder::findPath(int startX, int startY, int endX, int endY, Grid& grid)
{
    auto cmp = [](Node *a, Node *b) { return a->f - b->f; };
    BinaryHeap<Node *, decltype(cmp)> openList(cmp);

    Node *startNode = grid.getNodeAt(startX, startY);
    Node *endNode = grid.getNodeAt(endX, endY);

    startNode->g = 0;
    startNode->f = 0;

    openList.push(startNode);
    startNode->opened = true;

    while (!openList.empty())
    {
        Node *node = openList.pop();
        node->closed = true;

        if (node == endNode)
        {
            return util::backtrace(endNode);
        }

        auto neighbors = grid.getNeighbors(node, options_.diagonalMovement);
        for (Node *neighbor : neighbors)
        {
            if (neighbor->closed)
            {
                continue;
            }

            int x = neighbor->x;
            int y = neighbor->y;

            double ng = node->g + ((x - node->x == 0 || y - node->y == 0) ? 1.0 : SQRT2);

            if (!neighbor->opened || ng < neighbor->g)
            {
                neighbor->g = ng;
                neighbor->h = neighbor->h > 0 ? neighbor->h :
                                                options_.weight * heuristic_(std::abs(x - endX), std::abs(y - endY));
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = node;

                if (!neighbor->opened)
                {
                    openList.push(neighbor);
                    neighbor->opened = true;
                }
                else
                {
                    openList.updateItem(neighbor);
                }
            }
        }
    }

    return {};
}
} // namespace pathfinding
