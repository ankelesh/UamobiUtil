#pragma once
#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QPainter>

/*
	This class is realization of simple zebra-like delegate which is coloring odd rows in
	gray color. Use it as any other delegate.

	Update:
		Now this delegate checks for separating line in value. If it is found, delegate will paint it with blue

*/

class ZebraItemDelegate : public QStyledItemDelegate
{
public:
	ZebraItemDelegate(QObject* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};