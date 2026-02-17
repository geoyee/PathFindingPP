#ifndef PATH_FINDER_CONTROLLER_H
#define PATH_FINDER_CONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QPoint>
#include <QVector>
#include <QSet>

class GridModel;

class PathFinderController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsChanged)
    Q_PROPERTY(QStringList heuristicOptions READ heuristicOptions NOTIFY heuristicOptionsChanged)
    Q_PROPERTY(int currentAlgorithm READ currentAlgorithm WRITE setCurrentAlgorithm NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(int heuristic READ heuristic WRITE setHeuristic NOTIFY heuristicChanged)
    Q_PROPERTY(double weight READ weight WRITE setWeight NOTIFY weightChanged)
    Q_PROPERTY(bool allowDiagonal READ allowDiagonal WRITE setAllowDiagonal NOTIFY allowDiagonalChanged)
    Q_PROPERTY(bool biDirectional READ biDirectional WRITE setBiDirectional NOTIFY biDirectionalChanged)
    Q_PROPERTY(bool dontCrossCorners READ dontCrossCorners WRITE setDontCrossCorners NOTIFY dontCrossCornersChanged)
    Q_PROPERTY(double timeLimit READ timeLimit WRITE setTimeLimit NOTIFY timeLimitChanged)
    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged)
    Q_PROPERTY(int pathLength READ pathLength NOTIFY pathLengthChanged)
    Q_PROPERTY(int visitedCount READ visitedCount NOTIFY visitedCountChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool showHeuristic READ showHeuristic NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showOptions READ showOptions NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showWeight READ showWeight NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showAllowDiagonal READ showAllowDiagonal NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showBiDirectional READ showBiDirectional NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showDontCrossCorners READ showDontCrossCorners NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(bool showTimeLimit READ showTimeLimit NOTIFY currentAlgorithmChanged)

public:
    explicit PathFinderController(GridModel *gridModel, QObject *parent = nullptr);

    QStringList algorithms() const;
    QStringList heuristicOptions() const;
    int currentAlgorithm() const;
    void setCurrentAlgorithm(int algorithm);
    int heuristic() const;
    void setHeuristic(int h);
    double weight() const;
    void setWeight(double w);
    bool allowDiagonal() const;
    void setAllowDiagonal(bool allow);
    bool biDirectional() const;
    void setBiDirectional(bool bi);
    bool dontCrossCorners() const;
    void setDontCrossCorners(bool dont);
    double timeLimit() const;
    void setTimeLimit(double limit);
    bool isSearching() const;
    int pathLength() const;
    int visitedCount() const;
    QString statusMessage() const;
    bool showHeuristic() const;
    bool showOptions() const;
    bool showWeight() const;
    bool showAllowDiagonal() const;
    bool showBiDirectional() const;
    bool showDontCrossCorners() const;
    bool showTimeLimit() const;

    Q_INVOKABLE void findPath();
    Q_INVOKABLE void stopSearch();
    Q_INVOKABLE void clearPath();
    Q_INVOKABLE void retranslate();

Q_SIGNALS:
    void algorithmsChanged();
    void heuristicOptionsChanged();
    void currentAlgorithmChanged();
    void heuristicChanged();
    void weightChanged();
    void allowDiagonalChanged();
    void biDirectionalChanged();
    void dontCrossCornersChanged();
    void timeLimitChanged();
    void isSearchingChanged();
    void pathLengthChanged();
    void visitedCountChanged();
    void statusMessageChanged();
    void pathUpdated();

private:
    void setPathLength(int length);
    void setVisitedCount(int count);
    void setStatusMessage(const QString& message);
    void setIsSearching(bool searching);

    GridModel *m_gridModel;
    int m_currentAlgorithm = 0;
    int m_heuristic = 0;
    double m_weight = 1.0;
    bool m_allowDiagonal = false;
    bool m_biDirectional = false;
    bool m_dontCrossCorners = false;
    double m_timeLimit = 0.0;
    bool m_isSearching = false;
    int m_pathLength = 0;
    int m_visitedCount = 0;
    QString m_statusMessage;
};

#endif // !PATH_FINDER_CONTROLLER_H
