#include "CountingDelegate.h"
#include "networking/things.h"
#include "widgets/utils/ElementsStyles.h"


static QColor shadeOfGray(225, 226, 227);
void CountingItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	// drawing rectangle for first date
	int qty = index.data(DataEntityListModel::DirectAccess).value<RecEntity >()->getAttachedNumber();


	// painting begins
	painter->save();
	// drawing first textbox
	QRect textbox(option.rect.topLeft(), QSize(
		
			option.rect.width() * 0.85 , option.rect.height()));
	painter->setPen(Qt::PenStyle::SolidLine); 
	if (index.row() & 1)
	{
		painter->setBrush(QBrush(shadeOfGray));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white));
	}
	painter->setFont(option.font);
	painter->drawRect(textbox);
	painter->drawText(textbox, Qt::AlignLeft | Qt::AlignTop /*| Qt::TextWordWrap*/,
		index.data(DataEntityListModel::DirectAccess).value<RecEntity>()->getTitle());
	textbox.setTopLeft(textbox.topRight());
		textbox.setBottomRight(option.rect.bottomRight());
		painter->setBrush(StyleManager::instanse()->EnumerablesFieldsColor);
		painter->drawRect(textbox);
		painter->drawText(textbox, Qt::AlignCenter, QString::number(qty));
	
	// draw selection
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	painter->restore();
}
