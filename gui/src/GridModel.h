#ifndef GRID_MODEL_H
#define GRID_MODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QVector>
#include <QSet>

class GridModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QPoint startPoint READ startPoint NOTIFY startPointChanged)
    Q_PROPERTY(QPoint endPoint READ endPoint NOTIFY endPointChanged)

public:
    enum Roles
    {
        CellRole = Qt::UserRole + 1,
        XRole,
        YRole
    };

    enum CellType
    {
        Empty = 0,
        Wall = 1,
        Start = 2,
        End = 3,
        Path = 4,
        Visited = 5
    };
    Q_ENUM(CellType)

    explicit GridModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int width() const;
    int height() const;
    QPoint startPoint() const;
    QPoint endPoint() const;
    void setWidth(int width);
    void setHeight(int height);

    Q_INVOKABLE void toggleWall(int x, int y);
    Q_INVOKABLE void setStart(int x, int y);
    Q_INVOKABLE void setEnd(int x, int y);
    Q_INVOKABLE void clearPath();
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE void setPath(const QVector<QPoint>& path);
    Q_INVOKABLE void setVisited(const QSet<QPoint>& visited);
    Q_INVOKABLE bool isWall(int x, int y) const;
    Q_INVOKABLE void setWall(int x, int y, bool wall);

    std::vector<std::vector<int>> toMatrix() const;

Q_SIGNALS:
    void widthChanged();
    void heightChanged();
    void startPointChanged();
    void endPointChanged();

private:
    int indexFromPos(int x, int y) const;
    QPoint posFromIndex(int index) const;

    int m_width = 20;
    int m_height = 20;
    QVector<CellType> m_cells;
    QPoint m_startPoint{1, 1};
    QPoint m_endPoint{18, 18};
};

#endif // !GRID_MODEL_H
