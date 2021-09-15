#include <QModelIndex>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "treemodel.h"
#include "treeitem.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

TreeModel::~TreeModel()
{
    delete rootItem;
}




bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
             int role)
{
    return getItem(index)->setData(index.column(), value);
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                   const QVariant &value, int role)
{
    return rootItem->setData(section, value);
}

bool TreeModel::insertColumns(int position, int columns,
                   const QModelIndex &parent)
{
    return getItem(parent)->insertColumns(position, columns);
}

bool TreeModel::removeColumns(int position, int columns,
                   const QModelIndex &parent)
{
    return getItem(parent)->removeColumns(position, columns);
}

bool TreeModel::insertRows(int position, int rows,
                const QModelIndex &parent)
{
    return getItem(parent)->insertChildren(position, rows, 2);
}

bool TreeModel::removeRows(int position, int rows,
                const QModelIndex &parent)
{
        return getItem(parent)->removeChildren(position, rows);
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}




void TreeModel::addTopLevelItem(TreeItem* item)
{
    rootItem->appendChild(item);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
