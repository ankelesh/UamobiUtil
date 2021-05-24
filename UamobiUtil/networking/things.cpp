#include "things.h"
#include "widgets/utils/ElementsStyles.h"
#include <QFontMetrics>
#include <algorithm>


int countNormalizedHeights(QString line, QChar breaker = '\n')
{
	int heights = 1;
    QString::iterator point = std::find(line.begin(), line.end(), breaker);;
	QString::iterator previousPoint = line.begin();
	while (point != line.end())
	{
		if (FontAdapter::instanse()->howMuchCharacterFitsIntoScreen()
			< point - previousPoint)
		{
			++heights;
		}
		++heights;
		previousPoint = point;
		++point;
		point = std::find(point, line.end(), breaker);
	}
	return (heights == 1) ? 2 : heights;
}



DataEntityListModel::DataEntityListModel(const Records& data, QObject* parent)
	: QAbstractListModel(parent), innerList(data), heights()
{
	heights.reserve(data.count());
	for (int i = 0; i < data.count(); ++i)
	{
		heights.push_back(countNormalizedHeights(data.at(i)->getTitle()));
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
		return QVariant(QSize(FontAdapter::instanse()->howMuchCharacterFitsIntoScreen(), heights.at(index.row())));
	}
	return QVariant();
}

QVariant DataEntityListModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
	return QVariant();
}

void DataEntityListModel::insertData(const Records& data)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	heights.reserve(data.count());
	for (int i = 0; i < data.count(); ++i)
	{
		heights.push_back(countNormalizedHeights(data.at(i)->getTitle()));
	}
	endResetModel();
}

void DataEntityListModel::insertData(const Records& data, const QVector<int>& hts)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	heights.clear();
	heights << hts;
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
			heights[i] = countNormalizedHeights(e->getTitle());
		}
	}
}

void DataEntityListModel::reset()
{
	beginResetModel();
	innerList.clear();
	endResetModel();
}

QModelIndex DataEntityListModel::moveByArrow(int arrow, const QModelIndex& current) const
{
	if (!current.isValid())
		return index(0);
	int new_index = current.row();
	switch (arrow)
	{
	case Qt::Key_Up:
		--new_index;
		break;
	case Qt::Key_Down:
		++new_index;
		break;
	}
	if (new_index >= 0 && new_index < rowCount())
		return index(new_index);
	else
		return current;
}

RecEntity DataEntityListModel::getItemDirectly(const QModelIndex& index) const
{
	if (!index.isValid())
		return RecEntity();
	if (index.row() >= rowCount())
		return RecEntity();
	return innerList.at(index.row());
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
