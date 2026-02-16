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
            tr("IDA*"),
            tr("Breadth First Search"),
            tr("Best First Search"),
            tr("Dijkstra"),
            tr("Jump Point Search"),
            tr("Orthogonal Jump Point Search"),
            tr("Trace")};
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

bool PathFinderController::biDirectional() const
{
    return m_biDirectional;
}

void PathFinderController::setBiDirectional(bool bi)
{
    if (m_biDirectional == bi)
    {
        return;
    }
    m_biDirectional = bi;
    Q_EMIT biDirectionalChanged();
}

bool PathFinderController::dontCrossCorners() const
{
    return m_dontCrossCorners;
}

void PathFinderController::setDontCrossCorners(bool dont)
{
    if (m_dontCrossCorners == dont)
    {
        return;
    }
    m_dontCrossCorners = dont;
    Q_EMIT dontCrossCornersChanged();
}

double PathFinderController::timeLimit() const
{
    return m_timeLimit;
}

void PathFinderController::setTimeLimit(double limit)
{
    if (qFuzzyCompare(m_timeLimit, limit))
    {
        return;
    }
    m_timeLimit = limit;
    Q_EMIT timeLimitChanged();
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

bool PathFinderController::showHeuristic() const
{
    switch (m_currentAlgorithm)
    {
        case 0 :
        case 1 :
        case 3 :
        case 5 :
        case 6 :
        case 7 :
            return true;
        default :
            return false;
    }
}

bool PathFinderController::showOptions() const
{
    switch (m_currentAlgorithm)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 7 :
            return true;
        default :
            return false;
    }
}

bool PathFinderController::showWeight() const
{
    return m_currentAlgorithm == 0 || m_currentAlgorithm == 1;
}

bool PathFinderController::showAllowDiagonal() const
{
    switch (m_currentAlgorithm)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 7 :
            return true;
        default :
            return false;
    }
}

bool PathFinderController::showBiDirectional() const
{
    switch (m_currentAlgorithm)
    {
        case 0 :
        case 2 :
        case 3 :
        case 4 :
            return true;
        default :
            return false;
    }
}

bool PathFinderController::showDontCrossCorners() const
{
    switch (m_currentAlgorithm)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 7 :
            return true;
        default :
            return false;
    }
}

bool PathFinderController::showTimeLimit() const
{
    return m_currentAlgorithm == 1;
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
    opts.dontCrossCorners = m_dontCrossCorners;

    if (m_allowDiagonal)
    {
        if (m_dontCrossCorners)
        {
            opts.diagonalMovement = pathfinding::DiagonalMovement::OnlyWhenNoObstacles;
        }
        else
        {
            opts.diagonalMovement = pathfinding::DiagonalMovement::IfAtMostOneObstacle;
        }
    }
    else
    {
        opts.diagonalMovement = pathfinding::DiagonalMovement::Never;
    }

    pathfinding::util::Path path;

    switch (m_currentAlgorithm)
    {
        case 0 :
        {
            if (m_biDirectional)
            {
                pathfinding::BiAStarFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            else
            {
                pathfinding::AStarFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            break;
        }
        case 1 :
        {
            pathfinding::IDAStarFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 2 :
        {
            if (m_biDirectional)
            {
                pathfinding::BiBreadthFirstFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            else
            {
                pathfinding::BreadthFirstFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            break;
        }
        case 3 :
        {
            if (m_biDirectional)
            {
                pathfinding::BiBestFirstFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            else
            {
                pathfinding::BestFirstFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            break;
        }
        case 4 :
        {
            if (m_biDirectional)
            {
                pathfinding::BiDijkstraFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            else
            {
                pathfinding::DijkstraFinder finder(opts);
                path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            }
            break;
        }
        case 5 :
        case 6 :
        {
            pathfinding::JumpPointFinder finder(opts);
            path = finder.findPath(start.x(), start.y(), end.x(), end.y(), grid);
            break;
        }
        case 7 :
        {
            pathfinding::AStarFinder finder(opts);
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
    setStatusMessage(tr("Ready"));
    Q_EMIT algorithmsChanged();
    Q_EMIT heuristicOptionsChanged();
}
