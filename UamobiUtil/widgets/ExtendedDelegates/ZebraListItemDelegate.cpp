#include "ZebraListItemDelegate.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QItemDelegate(parent), fm(*AppFonts->general())
{
}

void ZebraItemDelegate::_drawRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.row() & 1)
	{
		painter->setBrush(QBrush(Qt::lightGray));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white));
	}
	painter->drawRect(option.rect);
}

void ZebraItemDelegate::_drawSelection(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
		painter->setOpacity(1);
	}
}

void ZebraItemDelegate::_drawSeparator(QPainter* painter, const QStyleOptionViewItem& option) const
{
	painter->save();
	painter->setBrush(Qt::blue);
	painter->setOpacity(0.8);
	painter->drawRect(option.rect);
	painter->setPen(Qt::PenStyle::DashLine);
	painter->drawLine(QPoint(option.rect.topLeft().x(), option.rect.topLeft().y() + option.rect.height() / 2),
		QPoint(option.rect.topRight().x(), option.rect.topRight().y() + option.rect.height() / 2));
	painter->restore();
}

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return;
	}
	painter->save();
	_drawRect(painter, option, index);
	_drawSelection(painter, option, index);
	painter->setFont(option.font);
    painter->drawText(option.rect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString());
	painter->restore();
}

QSize ZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
        QRect boundRect(0,0, option.rect.width(), calculateAdaptiveHeight(1));
        QRect res =  option.fontMetrics.boundingRect(boundRect, Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignTop, index.data(Qt::DisplayRole).toString());
        res.setHeight(res.height() + option.fontMetrics.height());
        return  res.size();
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
		painter->restore();
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
