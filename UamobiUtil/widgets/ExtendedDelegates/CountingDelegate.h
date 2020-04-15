#pragma once
#include "ZebraListItemDelegate.h"


class CountingItemDelegate : public ZebraItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
public:

	CountingItemDelegate(QObject* parent) : ZebraItemDelegate(parent) {}
};