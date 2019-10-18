#include "AbstractListSelectionWidget.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/qscroller.h>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
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
	QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
	this->setWordWrap(true);
#ifdef QT_VERSION5X
	QObject::connect(this, &QListWidget::itemClicked, this, &AbstractListSelectionWidget::itemSelectedFromList);
#else
	QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelectedFromList(QListWidgetItem*)));
#endif
}
