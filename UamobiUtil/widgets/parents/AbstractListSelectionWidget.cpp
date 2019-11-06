#include "AbstractListSelectionWidget.h"
// Qt 5 only imports
#include "widgets/utils/ElementsStyles.h"
#ifdef QT_VERSION5X
#include <QtWidgets/qscroller.h>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/ElementWidgets/ZebraListItemDelegate.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

void  AbstractListSelectionWidget::init()
{
	for (int i = 0; i < countElems(); ++i)
	{
		addItem(elemToString(i));

#ifdef DEBUG
		detrace_CYCLEEXPL("adding elem " << elemToString(i) << "by index" << i);
#endif
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
	this->setFont(makeFont(0.05));
	this->setWordWrap(true);
	ZebraItemDelegate* zd = new ZebraItemDelegate(this);
	setItemDelegate(zd);
#ifdef QT_VERSION5X
	QObject::connect(this, &QListWidget::itemClicked, this, &AbstractListSelectionWidget::itemSelectedFromList);
#else
	QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelectedFromList(QListWidgetItem*)));
#endif
}