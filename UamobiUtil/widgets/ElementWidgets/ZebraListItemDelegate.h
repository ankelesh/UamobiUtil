#pragma once
#ifdef QT_VERSION5X

#include <QtWidgets/QItemDelegate>
#else
#include <QtGui/QItemDelegate>
#endif
#include <QtGui/QPainter>

/*
	This class is realization of simple zebra-like delegate which is coloring odd rows in
	gray color. Use it as any other delegate.

	Update:
		Now this delegate checks for separating line in value. If it is found, delegate will paint it with blue

*/

class ZebraItemDelegate : public QItemDelegate
{
public:
	ZebraItemDelegate(QObject* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};
