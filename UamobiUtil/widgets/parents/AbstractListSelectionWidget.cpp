#include "AbstractListSelectionWidget.h"


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
}

