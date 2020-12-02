#include "CheckableDelegate.h"
#include "networking/things.h"


static QPixmap* cachedChI;
static QPixmap* cachedUchI;

void CheckableDelegate::cacheImages(QRect& newRect) const
{
	if (cachedChI == Q_NULLPTR)
	{
		cachedChI = new QPixmap(checkedImage.pixmap(newRect.size()));
		cachedUchI = new QPixmap(uncheckedImage.pixmap(newRect.size()));
		return;
	}
	else
	{
		if (abs(cachedChI->height() - newRect.height()) > 10)
		{
			*cachedChI =checkedImage.pixmap(newRect.size());
			*cachedUchI = uncheckedImage.pixmap(newRect.size());
			return;
		}
		return;
	}
}

CheckableDelegate::CheckableDelegate(
	QColor checked, QColor unchecked, QObject* parent)
	: QStyledItemDelegate(parent), 
	checkedColor(checked), uncheckedColor(unchecked),
	checkedImage(":/res/with.png"), uncheckedImage(":/res/without.png")
{
}

void CheckableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	RecEntity item = index.data(DataEntityListModel::DirectAccess).value<RecEntity>();
	if (item.isNull())
		return;
	painter->save();
	if (item->getAttachedNumber() == 1)
	{
		painter->setBrush(QBrush(checkedColor));
	}
	else
	{
		painter->setBrush(QBrush(uncheckedColor));
	}
	painter->drawRect(option.rect);
    QRect imgRect(option.rect.topLeft().x() + int(option.rect.height() * 0.1),
        option.rect.topLeft().y() + int(option.rect.height() * 0.1), int(option.rect.height() * 0.9),
        int(option.rect.height() * 0.9));
	cacheImages(imgRect);
	if (item->getAttachedNumber() == 1)
	{
		painter->drawPixmap(imgRect, *cachedChI);
	}
	else
	{
		painter->drawPixmap(imgRect, *cachedUchI);
	}
	imgRect.setTopLeft(QPoint(
        option.rect.topLeft().x() + int(option.rect.height() * 0.9),
		option.rect.topLeft().y()));
	imgRect.setBottomRight(option.rect.bottomRight());
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
		painter->setOpacity(1);
	}
	painter->drawText(imgRect,Qt::AlignRight | Qt::AlignVCenter | Qt::TextWordWrap, item->getTitle());
	painter->restore();
}

CheckableDelegate::~CheckableDelegate()
{
}
QSize CheckableDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
		return  QSize(option.rect.width(), index.data(Qt::SizeHintRole).toSize().height() * option.fontMetrics.height());
	}
	return QStyledItemDelegate::sizeHint(option, index);
}