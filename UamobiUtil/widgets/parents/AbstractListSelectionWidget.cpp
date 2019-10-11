#include "AbstractListSelectionWidget.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/qscroller.h>
#else
#endif

void  AbstractListSelectionWidget::init()
{
	for (int i = 0; i < countElems(); ++i)
	{
		addItem(elemToString(i));
	}
}

void AbstractListSelectionWidget::reload()
{
	clear();
	init();
}
AbstractListSelectionWidget::AbstractListSelectionWidget(QWidget* parent)
	: QListWidget(parent)
{
	this->setSelectionMode(QAbstractItemView::SingleSelection);
#ifdef QT_VERSION5X
	QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
#else

#endif
#ifdef QT_VERSION5X
	QObject::connect(this, &QListWidget::itemClicked, this, &AbstractListSelectionWidget::itemSelectedFromList);
#else
	QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelectedFromList(QListWidgetItem*)));
#endif
}