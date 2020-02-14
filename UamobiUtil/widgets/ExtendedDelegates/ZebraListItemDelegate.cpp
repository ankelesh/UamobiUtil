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
	painter->save();
	if (!index.isValid())
		return;
	if (index.row() % 2)
	{
		painter->setBrush(QBrush(Qt::lightGray));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white));
	}
	painter->drawRect(rct);
	painter->drawText(rct, Qt::AlignCenter | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString());
	painter->restore();
	
}

QSize ZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
		return QSize(option.rect.width(),
			option.fontMetrics.height() * index.data(Qt::SizeHintRole).toInt());
	return QSize(option.rect.width(), option.fontMetrics.height() * 2);
}
