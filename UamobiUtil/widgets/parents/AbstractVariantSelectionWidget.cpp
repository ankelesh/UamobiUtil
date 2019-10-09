#include "AbstractVariantSelectionWidget.h"

AbstractVariantSelectionWidget::AbstractVariantSelectionWidget(QWidget* parent, bool test)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this))
{
	this->setLayout(mainLayout);
	if (test)
	{
		userButtons.push_back(new specwidgets::indexedButton(1, this));
		userButtons.last()->setText("TEST");
	}
}

void AbstractVariantSelectionWidget::reload()
{
	QVector<specwidgets::indexedButton*>::iterator start = userButtons.begin();
	while (start != userButtons.end())
	{
		mainLayout->removeWidget(*start);
		delete (*start);
		++start;
	}
	userButtons.clear();
	init();
}

void AbstractVariantSelectionWidget::init()
{
	int elems = countElems();
	for (int i = 0; i < elems; ++i)
	{
		userButtons.push_back(new specwidgets::indexedButton(i, this));
		userButtons.last()->setText(elemAsString(i));
		mainLayout->addWidget(userButtons.last());
#ifdef QT_VERSION5X
		QObject::connect(userButtons.last(), &specwidgets::indexedButton::indexClicked, this, &AbstractVariantSelectionWidget::indexSelected);
#else
		QObject::connect(userButtons.last(), SIGNAL(indexClicked(int)), this, SLOT(indexSelected(int)));
#endif
	}
}