#include "GridModel.h"
#include "PathFinderController.h"

#include <pathfinding/PathFinding.h>

PathFinderController::PathFinderController(GridModel *gridModel, QObject *parent)
    : QObject(parent)
    , m_gridModel(gridModel)
{
    setStatusMessage(tr("Ready"));
}

QStringList PathFinderController::algorithms() const
{
    return {tr("A*"),
            tr("Breadth First"),
            tr("Dijkstra"),
            tr("Best First"),
            tr("Bi-A*"),
            tr("Bi-Breadth First"),
            tr("Bi-Dijkstra"),
            tr("Bi-Best First"),
            tr("IDA*"),
            tr("Jump Point")};
}

QStringList PathFinderController::diagonalOptions() const
{
    return {tr("Never"), tr("Always"), tr("If At Most One Obstacle"), tr("Only When No Obstacles")};
}

QStringList PathFinderController::heuristicOptions() const
{
    return {tr("Manhattan"), tr("Euclidean"), tr("Octile"), tr("Chebyshev")};
}

int PathFinderController::currentAlgorithm() const
{
    return m_currentAlgorithm;
}

void PathFinderController::setCurrentAlgorithm(int algorithm)
{
    if (m_currentAlgorithm == algorithm)
    {
        return;
    }
    m_currentAlgorithm = algorithm;
    Q_EMIT currentAlgorithmChanged();
}

int PathFinderController::diagonalMovement() const
{
    return m_diagonalMovement;
}

void PathFinderController::setDiagonalMovement(int movement)
{
    if (m_diagonalMovement == movement)
    {
        return;
    }
    m_diagonalMovement = movement;
    Q_EMIT diagonalMovementChanged();
}

int PathFinderController::heuristic() const
{
    return m_heuristic;
}

void PathFinderController::setHeuristic(int h)
{
    if (m_heuristic == h)
    {
        return;
    }
    m_heuristic = h;
    Q_EMIT heuristicChanged();
}

double PathFinderController::weight() const
{
    return m_weight;
}

void PathFinderController::setWeight(double w)
{
    if (qFuzzyCompare(m_weight, w))
    {
        return;
    }
    m_weight = w;
    Q_EMIT weightChanged();
}

bool PathFinderController::allowDiagonal() const
{
    return m_allowDiagonal;
}

void PathFinderController::setAllowDiagonal(bool allow)
{
    if (m_allowDiagonal == allow)
    {
        return;
    }
    m_allowDiagonal = allow;
    Q_EMIT allowDiagonalChanged();
}

bool PathFinderController::isSearching() const
{
    return m_isSearching;
}

int PathFinderController::pathLength() const
{
    return m_pathLength;
}

int PathFinderController::visitedCount() const
{
    return m_visitedCount;
}

QString PathFinderController::statusMessage() const
{
    return m_statusMessage;
}

void PathFinderController::findPath()
{
    if (m_isSearching)
    {
        return;
    }

    setIsSearching(true);
    setStatusMessage(tr("Searching..."));
    m_gridModel->clearPath();

    auto matrix = m_gridModel->toMatrix();
    auto start = m_gridModel->startPoint();
    auto end = m_gridModel->endPoint();

    pathfinding::Grid grid(matrix);
    pathfinding::FinderOptions opts;
    opts.weight = m_weight;
    opts.allowDiagonal = m_allowDiagonal;

    switch (m_diagonalMovement)
    {
        case 0 :
            opts.diagonalMovement = pathfinding::DiagonalMovement::Never;
            break;
        case 1 :
            opts.diagonalMovement = pathfinding::DiagonalMovement::Always;
            break;
        case 2 :
            opts.diagonalMovement = pathfinding::DiagonalMovement::IfAtMostOneObstacle;
            break;
        case 3 :
            opts.diagonalMovement = pathfinding::DiagonalMovement::OnlyWhenNoObstacles;
            break;
    }

    pathfinding::util::Path path;

    switch (m_currentAlgorithm)
    {
        case 0 :
        {
            pathfinding::AStarFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 1 :
        {
            pathfinding::BreadthFirstFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 2 :
        {
            pathfinding::DijkstraFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 3 :
        {
            pathfinding::BestFirstFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 4 :
        {
            pathfinding::BiAStarFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 5 :
        {
            pathfinding::BiBreadthFirstFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 6 :
        {
            pathfinding::BiDijkstraFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 7 :
        {
            pathfinding::BiBestFirstFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 8 :
        {
            pathfinding::IDAStarFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 9 :
        {
            pathfinding::JumpPointFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
    }

    QVector<QPoint> qPath;
    qPath.reserve(static_cast<int>(path.size()));
    for (const auto& p : path)
    {
        qPath.append(QPoint(p.first, p.second));
    }

    m_gridModel->setPath(qPath);
    setPathLength(static_cast<int>(path.size()));
    setVisitedCount(0);

    if (path.empty())
    {
        setStatusMessage(tr("No path found!"));
    }
    else
    {
        setStatusMessage(tr("Path found! Length: %1").arg(path.size()));
    }

    setIsSearching(false);
}

void PathFinderController::stopSearch()
{
    setIsSearching(false);
    setStatusMessage(tr("Search stopped"));
}

void PathFinderController::clearPath()
{
    m_gridModel->clearPath();
    setPathLength(0);
    setVisitedCount(0);
    setStatusMessage(tr("Path cleared"));
}

void PathFinderController::setPathLength(int length)
{
    if (m_pathLength == length)
    {
        return;
    }
    m_pathLength = length;
    Q_EMIT pathLengthChanged();
}

void PathFinderController::setVisitedCount(int count)
{
    if (m_visitedCount == count)
    {
        return;
    }
    m_visitedCount = count;
    Q_EMIT visitedCountChanged();
}

void PathFinderController::setStatusMessage(const QString& message)
{
    if (m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    Q_EMIT statusMessageChanged();
}

void PathFinderController::setIsSearching(bool searching)
{
    if (m_isSearching == searching)
    {
        return;
    }
    m_isSearching = searching;
    Q_EMIT isSearchingChanged();
}

void PathFinderController::retranslate()
{
    Q_EMIT algorithmsChanged();
    Q_EMIT diagonalOptionsChanged();
    Q_EMIT heuristicOptionsChanged();
}
