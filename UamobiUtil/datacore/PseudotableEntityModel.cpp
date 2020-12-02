#include "PseudotableEntityModel.h"
#include "widgets/utils/ElementsStyles.h"


inline int PseudotableEntityModel::_flatIndex(int col, int row) const
{
	return col + row * columns;
}

int PseudotableEntityModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count() / columns + innerList.count() % columns;
}

int PseudotableEntityModel::columnCount(const QModelIndex& parent) const
{
	if (innerList.count() < columns)
		return innerList.count();
	return columns;
}

QVariant PseudotableEntityModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	int row = _flatIndex(index.column(), index.row());
	if (row >= innerList.count())
		return QVariant();
	switch (role)
	{
	case Qt::DisplayRole:
		return innerList.at(row)->getTitle();
	case SearchRole:
	{
		QVariant temp;
		temp.setValue<RecEntity>(innerList.at(row));
		return temp;
	}
	case DataCopyRole:
	{
		QVariant temp;
		temp.setValue<RecEntity>(RecEntity(innerList.at(row)->clone()));
		return temp;
	}
	case DirectAccess:
	{
		QVariant temp;
		temp.setValue<RecEntity>(innerList.at(row));
		return temp;
	}
	case Qt::SizeHintRole:
		return QVariant(QSize(FontAdapter::instanse()->howMuchCharacterFitsIntoScreen(), heights.at(index.row())));
	}
	return QVariant();
}

QVariant PseudotableEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

#ifdef QT_VERSION5X
void PseudotableEntityModel::mapClickToEntity(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(_flatIndex(index.column(), index.row())));
}
#else
void PseudotableEntityModel::mapClickToEntity(QModelIndex index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(_flatIndex(index.column(), index.row())));
}
#endif
QModelIndex PseudotableEntityModel::moveByArrow(int arrow, const QModelIndex& current) const
{
	if (!current.isValid())
		return current;
	int new_indexX = current.row();
	int new_indexY = current.column();
	switch (arrow)
	{
	case Qt::Key_Up:
		--new_indexX;
		break;
	case Qt::Key_Down:
		++new_indexX;
		break;
	case Qt::Key_Left:
		--new_indexY;
		break;
	case Qt::Key_Right:
		++new_indexY;
		break;
	}
	if (new_indexX >= 0 && new_indexX < rowCount())
		if (new_indexY >= 0 && new_indexY < columnCount())
			return index(new_indexX, new_indexY);
	return current;
}
