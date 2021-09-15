#include <QtWidgets>
#include "treeitem.h"

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
    : itemData(data),
      parentItem(parent)
{}

TreeItem *TreeItem::parent()
{
    return parentItem;
}


TreeItem *TreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if(position < 0 || position >= columnCount())
        return false;

    for(int i = position; i < position + columns || i < columnCount(); ++i)
    {
        itemData[i].clear();
    }
    return true;
}

int TreeItem::columnCount() const
{
    return itemData.size();
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
    {
        return QVariant();
    }
    return itemData.at(column);
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if(column < 0 || column > itemData.size())
        return false;

    if(itemData.size() == column){
        itemData.emplace_back(value);
        return true;
    }
    itemData[column] = value;
    return true;
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (TreeItem *child : qAsConst(childItems))
        child->insertColumns(position, columns);

    return true;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}
