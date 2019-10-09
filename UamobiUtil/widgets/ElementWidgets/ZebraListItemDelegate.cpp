#include "ZebraListItemDelegate.h"

ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QRect rct = option.rect;
	if (index.row() % 2 == 1)
	{
		painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
	}
	else
	{
		painter->setBrush(option.palette.background());
	}
	if (index.data(Qt::DisplayRole).toString().startsWith("----"))
	{
		painter->setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
	}
	painter->drawRect(rct);
	QStyledItemDelegate::paint(painter, option, index);
}