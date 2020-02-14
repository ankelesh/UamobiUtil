#include "ControlManager.h"



abs_control* fabricateControl(QString initstr, QBoxLayout* layout, QWidget* parent)
{
	switch (legacy::guessControlType(initstr))
	{
	case ControlManager::Quantity:
	{	
		QuantityControl* qc = new QuantityControl(false, initstr, parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		return qc;
	}
	default:
		return Q_NULLPTR;
	}
}