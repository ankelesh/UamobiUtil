#pragma once
#include "ZebraListItemDelegate.h"


class CountingItemDelegate : public ZebraItemDelegate
{
	Q_OBJECT
protected:
	QFont quantityFont;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
public:

	CountingItemDelegate(QObject* parent);
};