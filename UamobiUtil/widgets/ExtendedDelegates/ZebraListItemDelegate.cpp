#include "ZebraListItemDelegate.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QItemDelegate(parent), fm(*AppFonts->general())
{
}

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	if (!index.isValid())
	{
		return;
	}
	if (index.row() & 1)
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
		return  QSize(option.rect.width(), index.data(Qt::SizeHintRole).toSize().height() * option.fontMetrics.height());
	}
	return QItemDelegate::sizeHint(option, index);
}

void CheckmateBoardDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	if (!index.isValid())
	{
		if ((index.row() + index.column()) & 1)
		{
			painter->setBrush(QBrush(Qt::lightGray));
		}
		else
		{
			painter->setBrush(QBrush(Qt::white));
		}
		painter->drawRect(option.rect);
		painter->drawText(option.rect,
			Qt::AlignCenter | Qt::TextWordWrap, "...");

		return;
	}
	if ((index.row() + index.column()) & 1)
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
	painter->setFont(option.font);
	painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString());
	painter->restore();
}
