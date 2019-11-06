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
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
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
	if (val >= 0 && val < countElems())
	{
		indexSelected(val);
		return true;
	}
	return false;
}

void AbstractVariantSelectionWidget::init()
{
	QFont scf = makeFont(0.05);
	int elems = countElems();
	for (int i = 0; i < elems; ++i)
	{
		userButtons.push_back(new specwidgets::indexedButton(i, this));
		userButtons.last()->setText(elemAsString(i));
		userButtons.last()->setFont(scf);
		mainLayout->addWidget(userButtons.last());
		if (i % 2) {
			userButtons.last()->setStyleSheet(ZEBRAODD_BUTTONS_STYLESHEET);
		}
		else
		{
			userButtons.last()->setStyleSheet(ZEBRAEVEN_BUTTONS_STYLESHEET);
		}
		userButtons.last()->setMinimumHeight(calculateAdaptiveButtonHeight());
#ifdef QT_VERSION5X
		QObject::connect(userButtons.last(), &specwidgets::indexedButton::indexClicked, this, &AbstractVariantSelectionWidget::indexSelected);
#else
		QObject::connect(userButtons.last(), SIGNAL(indexClicked(int)), this, SLOT(indexSelected(int)));
#endif
	}
}