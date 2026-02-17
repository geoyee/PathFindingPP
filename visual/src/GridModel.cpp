#include "GridModel.h"

GridModel::GridModel(QObject *parent) : QAbstractListModel(parent)
{
    m_cells.fill(Empty, m_width * m_height);
    m_cells[indexFromPos(m_startPoint.x(), m_startPoint.y())] = Start;
    m_cells[indexFromPos(m_endPoint.x(), m_endPoint.y())] = End;
}

int GridModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return static_cast<int>(m_cells.size());
}

QVariant GridModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_cells.size()))
    {
        return QVariant();
    }

    int idx = index.row();
    QPoint pos = posFromIndex(idx);

    switch (role)
    {
        case CellRole :
            return m_cells[idx];
        case XRole :
            return pos.x();
        case YRole :
            return pos.y();
        default :
            return QVariant();
    }
}

QHash<int, QByteArray> GridModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CellRole] = "cellType";
    roles[XRole] = "cellX";
    roles[YRole] = "cellY";
    return roles;
}

int GridModel::width() const
{
    return m_width;
}

int GridModel::height() const
{
    return m_height;
}

QPoint GridModel::startPoint() const
{
    return m_startPoint;
}

QPoint GridModel::endPoint() const
{
    return m_endPoint;
}

void GridModel::setWidth(int width)
{
    if (m_width == width)
    {
        return;
    }
    beginResetModel();
    m_width = width;
    m_cells.fill(Empty, m_width * m_height);
    m_startPoint = QPoint(1, 1);
    m_endPoint = QPoint(m_width - 2, m_height - 2);
    m_cells[indexFromPos(m_startPoint.x(), m_startPoint.y())] = Start;
    m_cells[indexFromPos(m_endPoint.x(), m_endPoint.y())] = End;
    m_path.clear();
    endResetModel();
    Q_EMIT widthChanged();
}

void GridModel::setHeight(int height)
{
    if (m_height == height)
    {
        return;
    }
    beginResetModel();
    m_height = height;
    m_cells.fill(Empty, m_width * m_height);
    m_startPoint = QPoint(1, 1);
    m_endPoint = QPoint(m_width - 2, m_height - 2);
    m_cells[indexFromPos(m_startPoint.x(), m_startPoint.y())] = Start;
    m_cells[indexFromPos(m_endPoint.x(), m_endPoint.y())] = End;
    m_path.clear();
    endResetModel();
    Q_EMIT heightChanged();
}

void GridModel::toggleWall(int x, int y)
{
    int idx = indexFromPos(x, y);
    if (idx < 0 || idx >= static_cast<int>(m_cells.size()))
    {
        return;
    }

    CellType& cell = m_cells[idx];
    if (cell == Empty)
    {
        cell = Wall;
    }
    else if (cell == Wall)
    {
        cell = Empty;
    }

    QVector<int> roles;
    roles.append(CellRole);
    Q_EMIT dataChanged(index(idx), index(idx), roles);
}

void GridModel::setStart(int x, int y)
{
    int idx = indexFromPos(x, y);
    if (idx < 0 || idx >= static_cast<int>(m_cells.size()))
    {
        return;
    }

    int oldIdx = indexFromPos(m_startPoint.x(), m_startPoint.y());
    m_cells[oldIdx] = Empty;
    QVector<int> roles;
    roles.append(CellRole);
    Q_EMIT dataChanged(index(oldIdx), index(oldIdx), roles);

    m_startPoint = QPoint(x, y);
    m_cells[idx] = Start;
    Q_EMIT dataChanged(index(idx), index(idx), roles);
    Q_EMIT startPointChanged();
}

void GridModel::setEnd(int x, int y)
{
    int idx = indexFromPos(x, y);
    if (idx < 0 || idx >= static_cast<int>(m_cells.size()))
    {
        return;
    }

    int oldIdx = indexFromPos(m_endPoint.x(), m_endPoint.y());
    m_cells[oldIdx] = Empty;
    QVector<int> roles;
    roles.append(CellRole);
    Q_EMIT dataChanged(index(oldIdx), index(oldIdx), roles);

    m_endPoint = QPoint(x, y);
    m_cells[idx] = End;
    Q_EMIT dataChanged(index(idx), index(idx), roles);
    Q_EMIT endPointChanged();
}

void GridModel::clearPath()
{
    m_path.clear();
}

void GridModel::clearAll()
{
    beginResetModel();
    m_cells.fill(Empty, m_width * m_height);
    m_startPoint = QPoint(1, 1);
    m_endPoint = QPoint(m_width - 2, m_height - 2);
    m_cells[indexFromPos(m_startPoint.x(), m_startPoint.y())] = Start;
    m_cells[indexFromPos(m_endPoint.x(), m_endPoint.y())] = End;
    m_path.clear();
    endResetModel();
}

void GridModel::setPath(const QVector<QPoint>& path)
{
    m_path = path;
}

bool GridModel::isWall(int x, int y) const
{
    int idx = indexFromPos(x, y);
    if (idx < 0 || idx >= static_cast<int>(m_cells.size()))
    {
        return true;
    }
    return m_cells[idx] == Wall;
}

void GridModel::setWall(int x, int y, bool wall)
{
    int idx = indexFromPos(x, y);
    if (idx < 0 || idx >= static_cast<int>(m_cells.size()))
    {
        return;
    }

    CellType& cell = m_cells[idx];
    QVector<int> roles;
    roles.append(CellRole);
    if (wall && cell == Empty)
    {
        cell = Wall;
        Q_EMIT dataChanged(index(idx), index(idx), roles);
    }
    else if (!wall && cell == Wall)
    {
        cell = Empty;
        Q_EMIT dataChanged(index(idx), index(idx), roles);
    }
}

std::vector<std::vector<int>> GridModel::toMatrix() const
{
    std::vector<std::vector<int>> matrix(m_height, std::vector<int>(m_width, 0));
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int idx = indexFromPos(x, y);
            matrix[y][x] = (m_cells[idx] == Wall) ? 1 : 0;
        }
    }
    return matrix;
}

QVariantList GridModel::getPathPoints() const
{
    QVariantList result;
    for (const QPoint& p : m_path)
    {
        QVariantMap point;
        point["x"] = p.x();
        point["y"] = p.y();
        result.append(point);
    }
    return result;
}

int GridModel::indexFromPos(int x, int y) const
{
    return y * m_width + x;
}

QPoint GridModel::posFromIndex(int index) const
{
    return QPoint(index % m_width, index / m_width);
}
