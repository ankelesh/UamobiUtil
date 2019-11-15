#include "ControlManager.h"

abs_control* fabricateControl(QString& assocBuffer, QString initstr, QBoxLayout* layout, QWidget* parent)
{
	switch (legacy::guessControlType(initstr))
	{
	case ControlManager::Quantity:
	{
		QuantityControl* qc = new QuantityControl(assocBuffer, initstr, parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		return qc;
	}
	default:
		return Q_NULLPTR;
	}
}

abs_control* fabricateControl(QString initstr, QBoxLayout* layout, QWidget* parent)
{
	switch (legacy::guessControlType(initstr))
	{
	case ControlManager::Quantity:
	{	
		QuantityControl* qc = new QuantityControl(initstr, parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		return qc;
	}
	default:
		return Q_NULLPTR;
	}
}