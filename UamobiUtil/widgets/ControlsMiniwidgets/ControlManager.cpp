#include "ControlManager.h"
#include "QuantityControl.h"
#include "DateTimeControl.h"
#include "LabelControl.h"
#include "BarcodeControl.h"
#include "StringControl.h"



abs_control* fabricateControl(InputControl control, QBoxLayout* layout, QWidget* parent)
{
	switch (control->getAttachedNumber())
	{
	case InputControlEntity::Float:
	{
		QuantityControl* qc = new QuantityControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		return qc;
	}
	case InputControlEntity::Int:
	{
		QuantityControl* qc = new QuantityControl(true, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		return qc;
	}
	case InputControlEntity::Date:
	{
		DateTimeControl* dc = new DateTimeControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, dc->myWidget());
		return dc;
	}
	case InputControlEntity::Label:
	{
		LabelControl* lc = new LabelControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, lc->myWidget());
		return lc;
	}
	case InputControlEntity::Barcode:
	{
		BarcodeControl* bc = new BarcodeControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, bc->myWidget());
		return bc;
	}
	case InputControlEntity::String:
	{
		StringControl* sc = new StringControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, sc->myWidget());
		return sc;
	}
	case InputControlEntity::None:
	default:
		return Q_NULLPTR;
	}
}
