#pragma once
#ifdef QT_VERSION5X

#include <QtWidgets/QItemDelegate>
#include <QtWidgets/qstyleditemdelegate.h>
#else
#include <QtGui/QItemDelegate>
#include <QtGui/QStyledItemDelegate>
#endif
#include <QtGui/QPainter>

/*
	Uses int value obtained from entity as checked flag
*/

class CheckableDelegate : public QStyledItemDelegate
{
private:
	QColor checkedColor;
	QColor uncheckedColor;
	QIcon checkedImage;
	QIcon uncheckedImage;

	void cacheImages(QRect& newRect) const;
public:
	CheckableDelegate(QColor checked,
		QColor unchecked, QObject* parent = Q_NULLPTR);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	~CheckableDelegate();
};
