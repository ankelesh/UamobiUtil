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
	painter->drawRect(option.rect); 
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
		painter->setOpacity(1);
	}
	painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString());
	painter->restore();
	
}

QSize ZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
		bool ok;
		int temp = index.data(Qt::SizeHintRole).toInt(&ok);
		if (ok)
			return QSize(option.rect.width(),
					option.fontMetrics.height()* temp);
	}
		return QSize(option.rect.width(), option.fontMetrics.height() * 2);
}
