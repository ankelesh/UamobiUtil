#include "things.h"

DataEntityListModel::DataEntityListModel(const Records& data, QObject* parent)
	: QAbstractListModel(parent), innerList(data)
{
}

int DataEntityListModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count();
}

QVariant DataEntityListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	switch (role)
	{
	case Qt::DisplayRole:
		return innerList.at(index.row())->getTitle();
	case SearchRole:
	{
		QVariant temp;
		temp.setValue<RecEntity>(innerList.at(index.row()));
		return temp;
	}
	case DataCopyRole:
	{
		QVariant temp;
		temp.setValue<RecEntity>(RecEntity(innerList.at(index.row())->clone()));
		return temp;
	}
	case DirectAccess:
		QVariant temp;
		temp.setValue<RecEntity>(innerList.at(index.row()));
		return temp;
	}
	return QVariant();
}

QVariant DataEntityListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void DataEntityListModel::setData(const Records& data)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	endResetModel();
}

void DataEntityListModel::removeDataEntity(const QModelIndex& mindex)
{
	if (!mindex.isValid())
		return;
	beginRemoveRows(mindex, mindex.row(), mindex.row());
	innerList.removeAt(mindex.row());
	endRemoveRows();
}

void DataEntityListModel::removeDataEntity(RecEntity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i)->isSame(&(*e)))
		{
			beginRemoveRows(createIndex(i, 0), i, i);
			innerList.removeAt(i);
			endRemoveRows();
		}
	}
}

void DataEntityListModel::replaceDataEntity(RecEntity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i)->isSame(&(*(e))))
		{
			innerList[i] = e;
		}
	}
}

void DataEntityListModel::reset()
{
	beginResetModel();
	innerList.clear();
	endResetModel();
}

void DataEntityListModel::mapClickToEntity(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(index.row()));
}

void DataEntityListModel::lookForEntity(const RecEntity e)
{
	AbsRecEntity* tofind = e.data();
	for (int i = 0; i < rowCount(); ++i)
	{
		if (innerList.at(i)->isSame(tofind))
		{
			emit dataEntityClicked(innerList.at(i));
			return;
		}
	}
}
