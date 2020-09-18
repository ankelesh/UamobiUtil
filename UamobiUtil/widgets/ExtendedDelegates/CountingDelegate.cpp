#include "CountingDelegate.h"
#include "networking/things.h"
#include "widgets/utils/ElementsStyles.h"

QString& normalizeFloatString(QString& fs)
{
    QString::iterator ch = fs.begin();
	
	int nullsToAppend = 0;
    while (ch != fs.end())
	{
		if (*ch == '.')
		{
            nullsToAppend = 4 - (fs.end() - ch);
			break;
		}
		++ch;
	}
	if (nullsToAppend > 0)
		fs = fs.leftJustified(fs.count() + nullsToAppend, '0');
	return fs;
}
static QColor shadeOfGray(225, 226, 227);
void CountingItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	// drawing rectangle for first date
	double qty = index.data(DataEntityListModel::DirectAccess).value<RecEntity >()->getAttachedNumber();


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
	painter->setFont(quantityFont);
	painter->drawText(textbox, Qt::AlignCenter | Qt::TextWrapAnywhere, normalizeFloatString(QString::number(qty)));
	
	// draw selection
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

CountingItemDelegate::CountingItemDelegate(QObject* parent)
	: ZebraItemDelegate(parent), quantityFont(AppFonts->makeCustomFont(0.03))
{
}
QSize CountingItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
		int hght = (index.data(Qt::SizeHintRole).toSize().height());

		return  QSize(option.rect.width(), ((hght> 3)? hght : 3) * option.fontMetrics.height());
	}
	return QItemDelegate::sizeHint(option, index);
}
