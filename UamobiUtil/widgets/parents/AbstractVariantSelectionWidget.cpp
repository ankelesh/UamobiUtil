#include "AbstractVariantSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif // DEBUG


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
bool AbstractVariantSelectionWidget::isExpectingControl(int val)
{
#ifdef DEBUG
	detrace_METHEXPL("got control avsw");
#endif
	if (val > 0 && val < countElems())
	{
		indexSelected(val);
		return true;
	}
	return false;
}


void AbstractVariantSelectionWidget::init()
{
	
	int elems = countElems();
	for (int i = 0; i < elems; ++i)
	{
		userButtons.push_back(new specwidgets::indexedButton(i, this));
		userButtons.last()->setText(elemAsString(i));
		mainLayout->addWidget(userButtons.last());

		userButtons.last()->setStyleSheet(countAdaptiveFont(0.04));
		userButtons.last()->setMinimumSize(calculateAdaptiveSize(0.2));
#ifdef QT_VERSION5X
		QObject::connect(userButtons.last(), &specwidgets::indexedButton::indexClicked, this, &AbstractVariantSelectionWidget::indexSelected);
#else
		QObject::connect(userButtons.last(), SIGNAL(indexClicked(int)), this, SLOT(indexSelected(int)));
#endif
	}
}