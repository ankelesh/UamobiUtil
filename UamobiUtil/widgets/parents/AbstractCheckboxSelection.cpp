#include "AbstractCheckboxSelection.h"
#include "widgets/utils/ElementsStyles.h"
void AbstractCheckboxSelection::indexSelected(int Index)
{
	states[Index] = userButtons.at(Index)->isChecked();
}

void AbstractCheckboxSelection::init()
{
	QFont scf = makeFont(0.05);
	int elems = countElems();
	for (int i = 0; i < elems; ++i)
	{
		userButtons.push_back(new specwidgets::toggledIndexedButton(nonT, isT, i, this));
		userButtons.last()->setText(elemAsString(i));
		userButtons.last()->setFont(scf);
		mainLayout->addWidget(userButtons.last());
		userButtons.last()->setStyleSheet(CHECKBOX_BUTTON_STYLESHEET);
		userButtons.last()->setCheckable(true);
		userButtons.last()->setChecked(states.at(i));
		userButtons.last()->setMinimumHeight(calculateAdaptiveButtonHeight());
#ifdef QT_VERSION5X
		QObject::connect(userButtons.last(), &specwidgets::toggledIndexedButton::indexClicked, this, &AbstractCheckboxSelection::indexSelected);
#else
		QObject::connect(userButtons.last(), SIGNAL(indexClicked(int)), this, SLOT(indexSelected(int)));
#endif
	}
}

AbstractCheckboxSelection::AbstractCheckboxSelection(QVector<bool>& States, QWidget* parent)
	: AbstractVariantSelectionWidget(parent), states(States), isT(":/res/with.png"), nonT(":/res/without.png")
{
}