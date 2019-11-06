#include "ZebraListItemDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QItemDelegate(parent)
{
}

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QRect rct = option.rect;
#ifdef DEBUG
	detrace_CYCLEEXPL("drawing item on index " << index.row() << " wa " << index.row() % 2);
#endif

	if (index.row() % 2)
	{
		painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white));
	}
	painter->drawRect(rct);
	QItemDelegate::paint(painter, option, index);
}