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
    Q_PROPERTY(QStringList diagonalOptions READ diagonalOptions NOTIFY diagonalOptionsChanged)
    Q_PROPERTY(QStringList heuristicOptions READ heuristicOptions NOTIFY heuristicOptionsChanged)
    Q_PROPERTY(int currentAlgorithm READ currentAlgorithm WRITE setCurrentAlgorithm NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(int diagonalMovement READ diagonalMovement WRITE setDiagonalMovement NOTIFY diagonalMovementChanged)
    Q_PROPERTY(int heuristic READ heuristic WRITE setHeuristic NOTIFY heuristicChanged)
    Q_PROPERTY(double weight READ weight WRITE setWeight NOTIFY weightChanged)
    Q_PROPERTY(bool allowDiagonal READ allowDiagonal WRITE setAllowDiagonal NOTIFY allowDiagonalChanged)
    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged)
    Q_PROPERTY(int pathLength READ pathLength NOTIFY pathLengthChanged)
    Q_PROPERTY(int visitedCount READ visitedCount NOTIFY visitedCountChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit PathFinderController(GridModel *gridModel, QObject *parent = nullptr);

    QStringList algorithms() const;
    QStringList diagonalOptions() const;
    QStringList heuristicOptions() const;
    int currentAlgorithm() const;
    void setCurrentAlgorithm(int algorithm);
    int diagonalMovement() const;
    void setDiagonalMovement(int movement);
    int heuristic() const;
    void setHeuristic(int h);
    double weight() const;
    void setWeight(double w);
    bool allowDiagonal() const;
    void setAllowDiagonal(bool allow);
    bool isSearching() const;
    int pathLength() const;
    int visitedCount() const;
    QString statusMessage() const;

    Q_INVOKABLE void findPath();
    Q_INVOKABLE void stopSearch();
    Q_INVOKABLE void clearPath();
    Q_INVOKABLE void retranslate();

Q_SIGNALS:
    void algorithmsChanged();
    void diagonalOptionsChanged();
    void heuristicOptionsChanged();
    void currentAlgorithmChanged();
    void diagonalMovementChanged();
    void heuristicChanged();
    void weightChanged();
    void allowDiagonalChanged();
    void isSearchingChanged();
    void pathLengthChanged();
    void visitedCountChanged();
    void statusMessageChanged();

private:
    void setPathLength(int length);
    void setVisitedCount(int count);
    void setStatusMessage(const QString& message);
    void setIsSearching(bool searching);

    GridModel *m_gridModel;
    int m_currentAlgorithm = 0;
    int m_diagonalMovement = 0;
    int m_heuristic = 0;
    double m_weight = 1.0;
    bool m_allowDiagonal = false;
    bool m_isSearching = false;
    int m_pathLength = 0;
    int m_visitedCount = 0;
    QString m_statusMessage;
};

#endif // !PATH_FINDER_CONTROLLER_H
