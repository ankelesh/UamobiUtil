#pragma once
#include "ZebraListItemDelegate.h"
#include "datacore/AbsEntityPrototype.h"

class CountingItemDelegate : public ZebraItemDelegate
{
	Q_OBJECT
protected:
	QFont quantityFont;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QRect drawNameRect(QPainter* painter, const QStyleOptionViewItem& option, bool isOdd) const;
	QRect & drawCountRect(QPainter* painter, const QStyleOptionViewItem& option, QRect& textbox) const;
	void _drawCountingRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index,RecEntity& ent) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
public:

	CountingItemDelegate(QObject* parent);
};