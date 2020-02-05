#include "CheckableDelegate.h"
#include "networking/things.h"


QPixmap* cachedChI;
QPixmap* cachedUchI;
QIcon checkedImage(":/res/with.png");
QIcon uncheckedImage(":/res/without.png");

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
	checkedColor(checked), uncheckedColor(unchecked)
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
	QRect imgRect(option.rect.height() * 0.1, 
		option.rect.height() * 0.1, option.rect.height() * 0.9,
		option.rect.height() * 0.9);
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
		option.rect.topLeft().x() + option.rect.height() * 0.9, 
		option.rect.topLeft().y()));
	imgRect.setBottomRight(option.rect.bottomRight());
	
	painter->drawText(imgRect,Qt::AlignRight | Qt::AlignVCenter | Qt::TextWordWrap, item->getTitle());
	painter->restore();
}

CheckableDelegate::~CheckableDelegate()
{
}
