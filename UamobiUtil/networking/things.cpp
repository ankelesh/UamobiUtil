#include "things.h"

DataEntityListModel::DataEntityListModel(const Records& data, QObject* parent)
	: QAbstractListModel(parent), innerList(data), heights()
{
	heights.reserve(data.count());
	int ct = 0;
	for (int i = 0; i < data.count(); ++i)
	{
		ct = data.at(i)->getTitle().count('\n') + 1;
		if (ct == 1)
			++ct;
		heights.push_back(ct);
	}
}

int DataEntityListModel::rowCount(const QModelIndex& /*parent*/) const
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
	{	
		QVariant temp;
		temp.setValue<RecEntity>(innerList.at(index.row()));
		return temp;
	}
	case Qt::SizeHintRole:
		return QVariant(heights.at(index.row()));
	}
	return QVariant();
}

QVariant DataEntityListModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
	return QVariant();
}

void DataEntityListModel::setData(const Records& data)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	heights.reserve(data.count());
	int ct = 0;
	for (int i = 0; i < data.count(); ++i)
	{
		ct = data.at(i)->getTitle().count('\n') + 1;
		if (ct == 1)
			++ct;
		heights.push_back(ct);
	}
	endResetModel();
}

void DataEntityListModel::removeDataEntity(const QModelIndex& mindex)
{
	if (!mindex.isValid())
		return;
	beginRemoveRows(mindex, mindex.row(), mindex.row());
    innerList.remove(mindex.row());
	heights.remove(mindex.row());
	endRemoveRows();
}

void DataEntityListModel::removeDataEntity(RecEntity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i)->isSame(&(*e)))
		{
			beginRemoveRows(createIndex(i, 0), i, i);
            innerList.remove(i);
			heights.remove(i);
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
			int ct = e->getTitle().count('\n') + 1;
			if (ct == 1) ++ct;
			heights[i] = ct;
		}
	}
}

void DataEntityListModel::reset()
{
	beginResetModel();
	innerList.clear();
	endResetModel();
}

#ifdef QT_VERSION5X

void DataEntityListModel::mapClickToEntity(const QModelIndex& index)
{
    if (!index.isValid())
        return;
    emit dataEntityClicked(innerList.at(index.row()));
}
#else

void DataEntityListModel::mapClickToEntity(QModelIndex index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(index.row()));
}
#endif
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
