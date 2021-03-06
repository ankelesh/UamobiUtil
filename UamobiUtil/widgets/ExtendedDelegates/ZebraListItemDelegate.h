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
	This uses scaling with obtaining height through Qt::SizeHintRole. If no such role, then
	2*font height is used.
*/

class ZebraItemDelegate : public QItemDelegate
{
protected:
	QFontMetrics fm;
	void _drawRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void _drawSelection(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void _drawSeparator(QPainter* painter, const QStyleOptionViewItem& option) const;
public:
	ZebraItemDelegate(QObject* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

class CheckmateBoardDelegate : public ZebraItemDelegate
{
private:
public:
	CheckmateBoardDelegate(QObject* parent) : ZebraItemDelegate(parent) {}
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

};